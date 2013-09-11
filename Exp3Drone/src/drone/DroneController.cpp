/*
 * DroneController.cpp
 *
 *  Created on: 27 Aug 2013
 *      Author: doturner
 */

#include "DroneController.h"
#include "navdata.h"

#include <QDebug>
#include <QTimer>

DroneController::DroneController()
{
	qDebug() << "[DroneController] Constructor start";

	mConnectedToDrone = false;
	mConnectState = 0;
	m_pNavDataHandler = 0;
	m_pAtCommand = 0;

	m_pFtp = new QFtp(this);

	bool success = connect(this, SIGNAL(connected(QString)), this, SLOT(handleConnected()));

	if (!success)
	{
		qDebug() << "[DroneController] Unable to connect 'connected' signal with 'handleConnected' slot";
	}

	qDebug() << "[DroneController] Constructor end";
}

DroneController::~DroneController()
{
	qDebug() << "[DroneController] Destructor start";

	delete m_pFtp;
	m_pFtp = 0;

	if (m_pNavDataHandler != 0) delete m_pNavDataHandler;
	if (m_pAtCommand != 0) delete m_pAtCommand;

	qDebug() << "[DroneController] Destructor end";
}

void DroneController::connectToDrone()
{

	emit connecting();

	//emit statusChanged("Searching for AR.Drone");

	QTimer::singleShot(1, this, SLOT(connectToFtp()));
}

/*!
 Attempts to connect to the Drone's FTP server.
 */
void DroneController::connectToFtp()
{

	m_pFtp->connectToHost(DRONE_IP_ADDRESS, FTP_PORT);
	connect(m_pFtp, SIGNAL(readyRead()), this, SLOT(readFtp()));

	m_pFtp->login("anonymous");
	m_pFtp->get("version.txt");
	m_pFtp->close();

	// Set a timeout timer in case FTP get fails.
	QTimer::singleShot(5000, this, SLOT(ftpConnectionTimeout()));
}

/*!
 This function is called when the FTP get returns with version.txt.
 */
void DroneController::readFtp()
{
	qDebug() << "[DroneController] readFtp - Connected to Drone";
	// FTP answer from the drone. This means we are connected!
	mConnectedToDrone = true;
	mFirmwareVersion = m_pFtp->readAll();
	emit connected(mFirmwareVersion);

	//emit statusChanged(QString("Drone connected. Firmware: %1").arg(mVersion));
}

/*!
 This function is called if the FTP get takes too long answering.
 */
void DroneController::ftpConnectionTimeout()
{
	if (!mConnectedToDrone)
	{
		qDebug() << "[Loader] FTP to drone timed out.";
		emit failedToConnect("FTP to drone timed out");
	}
}

void DroneController::handleConnected()
{
	qDebug() << "[DroneController] handleConnected - start";

	//m_pNavDataHandler = new NavDataHandler(this);
	m_pAtCommand = new AtCommand(this);

	m_pAtCommand->sendInitSequence(1);
	m_pAtCommand->sendInitSequence(2);

	m_pAtCommand->start(QThread::HighPriority);

	//bool success = connect(m_pNavDataHandler, SIGNAL(updateARDroneState(uint)), m_pAtCommand, SLOT(updateARDroneState(uint)));
	//bool success2 = connect(m_pNavDataHandler, SIGNAL(sendInitSequence(int)), m_pAtCommand, SLOT(sendInitSequence(int)));

	//if (!success || !success2){
	//	qDebug() << "[DroneController] handleConnected - Failed to connect the nav data handler with the AtCommand";
	//}
}

void DroneController::takeOff()
{
	if (mConnectedToDrone){
		qDebug() << "[DroneController] Sending take off command";
		m_pAtCommand->startPressed();
	}
}

void DroneController::land()
{
	if (mConnectedToDrone){
		m_pAtCommand->stopPressed();
	}
}

void DroneController::setThrottleAndYaw(float throttle, float yaw){

	if(mConnectedToDrone)
	{
		qDebug() << "[DroneController] Setting throttle to: " << throttle << " and yaw to: " << yaw;
		m_pAtCommand->setThrottleAndYaw(throttle, yaw);
	}
}

void DroneController::setPitchAndRoll(float pitch, float roll){

	if(mConnectedToDrone)
	{
		m_pAtCommand->setPitchAndRoll(pitch, roll);
	}
}

void DroneController::emergencyPressed(){

	if (mConnectedToDrone)
	{
		m_pAtCommand->emergencyPressed();
	}
}

void DroneController::resetPressed(){

	if (mConnectedToDrone)
	{
		m_pAtCommand->resetPressed();
	}
}

void DroneController::changeCamera()
{
	if (mConnectedToDrone)
	{
		m_pAtCommand->changeCamera();
	}
}
