/*
 * DroneController.cpp
 *
 *  Created on: 9 Aug 2013
 *      Author: doturner
 */

#include "DroneController.h"
#include "navdata.h"


#include <QDebug>
#include <QMutexLocker>

DroneController::DroneController() {

	mCommandCount = 0;

	//lookUpHost();
	/*if (pControlSocket->bind(QHostAddress::Any, DRONE_CONTROL_PORT)) {

		qDebug() << "Opened socket to drone";
	} else {
		qDebug() << "FAILED to open socket to drone";
	}*/

	//Create a UDP socket for sending commands to the drone
	mUdpSocketCommandData = new QUdpSocket(this);
}

DroneController::~DroneController() {

	delete (mUdpSocketNavData);
}

/*!
    Looks up the AR.Drone host to check if we are connected to the drone.
*/
void DroneController::lookUpHost()
{
    mFtp = new QFtp(this);
    mFtp->connectToHost(WIFI_MYKONOS_IP, FTP_PORT);
    connect(mFtp, SIGNAL(readyRead()),this, SLOT(readDroneFtp()));

    mFtp->login("anonymous");
    mFtp->get("version.txt");
    mFtp->close();
    // Set a timeout timer in case FTP get fails.
    QTimer::singleShot(5000, this, SLOT(droneConnectionTimeout()));
}

/*!
    This function is called when the FTP get returns with version.txt.
*/
void DroneController::readDroneFtp()
{
    // FTP answer from the drone. This means we are connected!
    mConnectedToDrone = true;

    mVersion = mFtp->readAll();
    qDebug() << "[DroneController] CONNECTED. Drone Firmware Version: " << mVersion;

    emit droneConnected(mVersion);
}

/*!
    This function is called if the FTP get takes too long answering.
*/
void DroneController::droneConnectionTimeout()
{
    // Check if we have a connection. Maybe the user is just waiting a long time before pressing
    // a button.
    if (!mConnectedToDrone) {
        qDebug() << "[DroneController] FTP to drone timed out.";
        mLoaderTimer->stop();
    }
}


/*!
    This function sets up a UDP socket and sends a message to the drone that we are ready to
    receive data.
*/
void DroneController::initialize()
{
    qDebug() << "[DroneController] Initialize";

    QMutexLocker locker(&mMutex);

    mDroneResponded = false;
    mDroneBooting = true;

    // UDP socket for receiving data from the drone.
    mUdpSocketNavData = new QUdpSocket(this);
    bool res = mUdpSocketNavData->bind(NAVDATA_PORT, QUdpSocket::ShareAddress);
    if (!res) {
        qDebug() << "[DroneController] Error connecting to Navdata " << " result:" << res;
    } else {
        connect(mUdpSocketNavData, SIGNAL(readyRead()),this, SLOT(newNavDataReady()));
    }

    // Send message to drone to signal that we are ready to receive data.
    QHostAddress reciever;
    int res2;
    const char data[] = "\1\0\0\0";
    reciever = QHostAddress(WIFI_MYKONOS_IP);
    res2 = mUdpSocketNavData->writeDatagram(data,reciever,NAVDATA_PORT);
    if (res2 == -1) {
        qDebug() << "[DroneController] Error initializing NavData UDP socket.";
    } else {
    	qDebug() << "[DroneController] Sent 'ready to receive data' signal to drone (" << data << "). Result: " << res2;
    }

    mInitTimer = new QTimer(this);
    connect(mInitTimer, SIGNAL(timeout()), this, SLOT(initTimer()));
    mInitTimer->start(200);

}

/*!
    Slot called whenever new data is ready from the drone.
    Starts the thread with NormalPriority unless it is already running.
*/
void DroneController::newNavDataReady()
{
    int size = mUdpSocketNavData->pendingDatagramSize();
    QByteArray datagram;
    datagram.resize(size);
    mUdpSocketNavData->readDatagram(datagram.data(), size);
    mMutex.lock();
    mDatagramList.append(datagram);
    mMutex.unlock();

    qDebug() << "Datagram received: " << datagram;
    /*if (!isRunning()) {
        mDroneResponded = true;
        start(NormalPriority);
    } else {
        mCondition.wakeOne();
    }*/
}

void DroneController::initTimer()
{
    if (!mDroneResponded) {
        QHostAddress reciever;
        const char data[] = "\1\0\0\0";
        reciever = QHostAddress(WIFI_MYKONOS_IP);
        int res = mUdpSocketNavData->writeDatagram(data,reciever,NAVDATA_PORT);
        if (res == -1) {
            qDebug() << "[DroneController] Error initializing NavData UDP socket.";
        } else {
        	qDebug() << "[DroneController] Sent 'ready to receive data' signal to drone (" << data << "). Result: " << res;
        }
    } else {
        mInitTimer->stop();
    }
}

void DroneController::takeOff() {

	//Make the drone take off
	qDebug() << "Sending TAKE OFF command";

	QString takeOffCommand("AT*REF=%1,290718208\r");
	takeOffCommand = takeOffCommand.arg(++mCommandCount);

	qDebug() << "Sending command string: " << takeOffCommand;
	QByteArray data = takeOffCommand.toLatin1();

	mUdpSocketCommandData->writeDatagram(data, QHostAddress(WIFI_MYKONOS_IP), AT_PORT);
}

void DroneController::land() {

	//Make the drone land
	qDebug() << "Sending LAND command";

	QString takeOffCommand("AT*REF=%1,290717696\r");
		takeOffCommand = takeOffCommand.arg(++mCommandCount);

		qDebug() << "Sending command string: " << takeOffCommand;
		QByteArray data = takeOffCommand.toLatin1();

		mUdpSocketCommandData->writeDatagram(data, QHostAddress(WIFI_MYKONOS_IP), AT_PORT);

}

