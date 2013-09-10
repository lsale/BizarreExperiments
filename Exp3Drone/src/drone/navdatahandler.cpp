#include "navdatahandler.h"
#include "navdata.h"
//#include "navdata4.h"
#include "navdata5.h"

#include <QTimer>

/*!
 Constructs the NavDataHandler.
 The \a parent parameter is sent to the QThread constructor.
 */
NavDataHandler::NavDataHandler(QObject *parent) :
		QThread(parent)
{
	qDebug() << "[NavDataHandler] Constructor - start";
	mAbort = false;
	initialize();

	qDebug() << "[NavDataHandler] Constructor - end";
}

/*!
 Destructor.
 Waits for thread to abort before destroying the \c NavDataHandler object.
 */
NavDataHandler::~NavDataHandler()
{
	qDebug() << "[NavDataHandler] Destructor";
	mMutex.lock();
	mAbort = true;
	mCondition.wakeOne();
	mMutex.unlock();
	wait();
}

/*!
 This function sets up a UDP socket and sends a message to the drone that we are ready to
 receive data.
 */
void NavDataHandler::initialize()
{
	qDebug() << "[NavDataHandler] Initialize";

	QMutexLocker locker(&mMutex);

	mDroneResponded = false;
	mDroneBooting = true;

	// UDP socket for receiving data from the drone.
	mUdpSocketNavData = new QUdpSocket(this);
	bool res = mUdpSocketNavData->bind(NAVDATA_PORT, QUdpSocket::ShareAddress);
	if (!res)
	{
		qDebug() << "[NavDataHandler] Error connecting to Navdata " << " result:" << res;
	}
	else
	{
		connect(mUdpSocketNavData, SIGNAL(readyRead()), this, SLOT(newNavDataReady()));
	}

	// Send message to drone every 200ms to signal that we are ready to receive data.
	mInitTimer = new QTimer(this);
	connect(mInitTimer, SIGNAL(timeout()), this, SLOT(sendReadySignal()));
	mInitTimer->start(200);
}

/*!
 Thread main function.
 Reads status data received from the drone.
 */
void NavDataHandler::run()
{
	qDebug() << "[NavDataHandler] run";

	uint sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
	uint droneState = 0;
	bool emergencyState = false;
	uint batteryLevel = 4;

	forever
	{
		qDebug() << "[NavDataHandler] Entering main event loop";

		// Do the stuff but check if we need to abort first...
		if (mAbort)
		{
			return;
		}

		QByteArray datagram;

		// Copy the next datagram in the list to a variable that is local to the thread.
		mMutex.lock();
		if (!mDatagramList.empty())
		{
			datagram = mDatagramList.takeFirst();
		}
		mMutex.unlock();

		int size = datagram.size();

		if (size == 0)
		{
			qDebug() << "[NavDataHandler] datagram size = 0";
			sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
		}
		else
		{
			navdata_t* navdata = (navdata_t*) datagram.data();
			if (navdata->header == NAVDATA_HEADER)
			{
				if (get_mask_from_state(navdata->ardrone_state, ARDRONE_NAVDATA_BOOTSTRAP))
				{
					qDebug() << "[NavDataHandler] ARDRONE_NAVDATA_BOOTSTRAP";
					mDroneBooting = true;
					emit sendInitSequence(1);
				}
				else if (mDroneBooting && get_mask_from_state(navdata->ardrone_state, ARDRONE_COMMAND_MASK))
				{
					qDebug() << "[NavDataHandler] ARDRONE_COMMAND_MASK";
					mDroneBooting = false;
					emit sendInitSequence(2);
				}
				else
				{
					// If drone status has changed, signal it to the other components.
					if (droneState != navdata->ardrone_state)
					{
						droneState = navdata->ardrone_state;
						emit updateARDroneState(droneState);
					}

					// If emergency state has changed, signal it to the other components.
					if (emergencyState != get_mask_from_state(droneState, ARDRONE_EMERGENCY_MASK))
					{
						qDebug() << "[NavDataHandler] ARDRONE_EMERGENCY_MASK";
						emergencyState = get_mask_from_state(droneState, ARDRONE_EMERGENCY_MASK);
						emit updateEmergencyState(emergencyState);
					}

					// If the drone communication watchdog has been active then the sequence numbering
					// has been restarted.
					if (get_mask_from_state(droneState, ARDRONE_COM_WATCHDOG_MASK))
					{
						qDebug() << "[NavDataHandler] ARDRONE_COM_WATCHDOG_MASK";
						sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
					}

					// Check if this is not old data we have received.
					if (navdata->sequence > sequence)
					{
						quint32 navdata_cks = 0;

						navdata_unpacked_5_t navdata_unpacked;
						bool ret = ardrone_navdata_unpack_all_5(&navdata_unpacked, navdata, &navdata_cks);
						if (ret)
						{
							// Compute checksum.
							quint32 cks = navdata_compute_cks((quint8*) datagram.data(), size - sizeof(navdata_cks_t));

							// Compare checksums to see if data is valid.
							if (cks == navdata_cks)
							{
								// Data is valid.
								// Battery level is the only information we are using from this structure.
								uint tmpBatLevel = 0;
								uint tmpBat = navdata_unpacked.navdata_demo.vbat_flying_percentage;
								if (tmpBat > 75)
								{
									tmpBatLevel = 3;
								}
								else if (tmpBat > 50)
								{
									tmpBatLevel = 2;
								}
								else if (tmpBat > 25)
								{
									tmpBatLevel = 1;
								}
								else
								{
									tmpBatLevel = 0;
								}

								if (tmpBatLevel != batteryLevel)
								{
									batteryLevel = tmpBatLevel;
									emit updateBatteryLevel(batteryLevel);
								}
							}
							else
							{
								qDebug() << "[NavDataHandler] Checksum failed!";
							}
						}

					}
					sequence = navdata->sequence;
				}
			}
		}

		// If we have more data in queue, then wait 10 milliseconds before continuing.
		// Else wait until new data arrives.
		/*mMutex.lock();
		if (mDatagramList.count() > 0)
		{
			mCondition.wait(&mMutex, 10);
		}
		else
		{
			mCondition.wait(&mMutex);
		}
		mMutex.unlock();*/
	}
}

/*!
 Slot called whenever new data is ready from the drone.
 Starts the thread with NormalPriority unless it is already running.
 */
void NavDataHandler::newNavDataReady()
{
	int size = mUdpSocketNavData->pendingDatagramSize();
	QByteArray datagram;
	datagram.resize(size);
	mUdpSocketNavData->readDatagram(datagram.data(), size);
	mMutex.lock();
	mDatagramList.append(datagram);
	mMutex.unlock();
	if (!isRunning())
	{
		qDebug() << "[NavDataHandler] newNavDataRead - Drone responded by sending navigation data";
		mDroneResponded = true;
		start(NormalPriority);
	}
	else
	{
		mCondition.wakeOne();
	}
}

void NavDataHandler::sendReadySignal()
{
	if (!mDroneResponded)
	{
		QHostAddress reciever;
		const char data[] = "\1\0\0\0";
		reciever = QHostAddress(DRONE_IP_ADDRESS);
		int res = mUdpSocketNavData->writeDatagram(data, reciever, NAVDATA_PORT);
		if (res == -1)
		{
			qDebug() << "[NavDataHandler] Error initializing NavData UDP socket.";
		}
	}
	else
	{
		mInitTimer->stop();
	}
}
