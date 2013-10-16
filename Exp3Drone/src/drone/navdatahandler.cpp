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
	qDebug() << "[NavDataHandler] initialize - start";

	QMutexLocker locker(&mMutex);

	mDroneResponded = false;
	mDroneBooting = true;

	// UDP socket for receiving data from the drone.
	mUdpSocketNavData = new QUdpSocket(this);
	bool res = mUdpSocketNavData->bind(NAVDATA_PORT, QUdpSocket::ShareAddress);
	if (!res)
	{
		qDebug() << "[NavDataHandler] initialize - Error connecting to Navdata " << " result:" << res;
	}
	else
	{
		qDebug() << "[NavDataHandler] initialize - Successfully bound to nav data port: " << NAVDATA_PORT;
		connect(mUdpSocketNavData, SIGNAL(readyRead()), this, SLOT(newNavDataReady()));
	}

	// Send message to drone every 200ms to signal that we are ready to receive data.
	qDebug() << "[NavDataHandler] initialize - Sending data to drone every 200ms to let it know we're ready to receive data";

	mInitTimer = new QTimer(this);
	connect(mInitTimer, SIGNAL(timeout()), this, SLOT(sendReadySignal()));
	mInitTimer->start(200);

	qDebug() << "[NavDataHandler] initialize - End";
}

/*!
 Thread main function.
 Reads status data received from the drone.
 */
void NavDataHandler::run()
{
	qDebug() << "[NavDataHandler] run - start";

	uint sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
	uint droneState = 0;
	bool emergencyState = false;
	uint batteryLevel = 4;

	forever
	{
		qDebug() << "[NavDataHandler] run - Entering main event loop";

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

		qDebug() << "[NavDataHandler] run - Got datagram. Size: " << size;

		if (size == 0)
		{
			sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
			qDebug() << "[NavDataHandler] run - Size is zero. Sequence set to: " << sequence;
		}
		else
		{
			navdata_t* navdata = (navdata_t*) datagram.data();
			if (navdata->header == NAVDATA_HEADER)
			{
				if (get_mask_from_state(navdata->ardrone_state, ARDRONE_NAVDATA_BOOTSTRAP))
				{
					qDebug() << "[NavDataHandler] run - Drone state is: ARDRONE_NAVDATA_BOOTSTRAP (the drone is booting)";
					mDroneBooting = true;
					emit sendInitSequence(1);
				}
				else if (mDroneBooting && get_mask_from_state(navdata->ardrone_state, ARDRONE_COMMAND_MASK))
				{
					qDebug() << "[NavDataHandler] run - Drone state is: ARDRONE_COMMAND_MASK (command received)";
					mDroneBooting = false;
					emit sendInitSequence(2);
				}
				else
				{
					qDebug() << "[NavDataHandler] run - Drone state: " << navdata->ardrone_state;

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
						sequence = NAVDATA_SEQUENCE_DEFAULT - 1;
						qDebug() << "[NavDataHandler] run - ARDRONE_COM_WATCHDOG_MASK is set. Sequence now: " << sequence;
					}

					// Check if this is not old data we have received.
					if (navdata->sequence > sequence)
					{
						quint32 navdata_cks = 0;

						navdata_unpacked_5_t navdata_unpacked;
						qDebug() << "[NavDataHandler] run - Unpacking the navdata";
						bool ret = ardrone_navdata_unpack_all_5(&navdata_unpacked, navdata, &navdata_cks);
						if (ret)
						{
							qDebug() << "[NavDataHandler] run - Successfully unpacked the navdata";
							// Compute checksum.
							quint32 cks = navdata_compute_cks((quint8*) datagram.data(), size - sizeof(navdata_cks_t));

							qDebug() << "[NavDataHandler] run - Calculated checksum: " << cks;

							// Compare checksums to see if data is valid.
							if (cks == navdata_cks)
							{
								qDebug() << "[NavDataHandler] run - Our checksum matches the navdata checksum, data is valid";

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

								qDebug() << "[NavDataHandler] run - Battery level is: " << batteryLevel;
							}
							else
							{
								qDebug() << "[NavDataHandler] run - Checksum failed!";
							}
						}

					}
					sequence = navdata->sequence;

					qDebug() << "[NavDataHandler] run - Navdata sequence: " << sequence;
				}
			}
		}

		// If we have more data in queue, then wait 10 milliseconds before continuing.
		// Else wait until new data arrives.
		mMutex.lock();
		if (mDatagramList.count() > 0)
		{
			qDebug() << "[NavDataHandler] run - There is more data in the queue, waiting 10ms";
			mCondition.wait(&mMutex, 10);
		}
		else
		{
			qDebug() << "[NavDataHandler] run - No more data in queue, waiting for mMutex signal to process more data";
			mCondition.wait(&mMutex);
		}
		mMutex.unlock();
	}
}

/*!
 Slot called whenever new data is ready from the drone.
 Starts the thread with NormalPriority unless it is already running.
 */
void NavDataHandler::newNavDataReady()
{
	qDebug() << "[NavDataHandler] newNavDataReady - start";
	int size = mUdpSocketNavData->pendingDatagramSize();
	QByteArray datagram;
	datagram.resize(size);
	mUdpSocketNavData->readDatagram(datagram.data(), size);
	mMutex.lock();
	mDatagramList.append(datagram);
	mMutex.unlock();
	if (!isRunning())
	{
		qDebug() << "[NavDataHandler] newNavDataReady - Drone responded by sending navigation data, starting thread";
		mDroneResponded = true;
		start(NormalPriority);
	}
	else
	{
		qDebug() << "[NavDataHandler] newNavDataReady - Waking up the thread to allow datagram to be processed";
		mCondition.wakeOne();
	}

	qDebug() << "[NavDataHandler] newNavDataReady - end";
}

void NavDataHandler::sendReadySignal()
{
	qDebug() << "[NavDataHandler] sendReadySignal - start";

	if (!mDroneResponded)
	{

		qDebug() << "[NavDataHandler] sendReadySignal - Drone has not yet responded, sending ready signal";

		QHostAddress reciever;
		const char data[] = "\1\0\0\0"; //This is the ready signal
		reciever = QHostAddress(DRONE_IP_ADDRESS);
		int res = mUdpSocketNavData->writeDatagram(data, reciever, NAVDATA_PORT);
		if (res == -1)
		{
			qDebug() << "[NavDataHandler] sendReadySignal - Error initializing NavData UDP socket.";
		}
	}
	else
	{
		qDebug() << "[NavDataHandler] sendReadySignal - Drone has responded, will now stop sending ready signals";
		mInitTimer->stop();
	}

	qDebug() << "[NavDataHandler] sendReadySignal - end";

}
