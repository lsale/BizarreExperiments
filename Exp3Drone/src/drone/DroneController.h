/*
 * DroneController.h
 *
 *  Created on: 27 Aug 2013
 *      Author: doturner
 */

#ifndef DRONECONTROLLER_H_
#define DRONECONTROLLER_H_

#include "navdatahandler.h"
#include "atcommand.h"
#include "../GamePadHandler.h"

#include <QObject>
#include <QFtp>

class DroneController : public QObject
{
	Q_OBJECT
public:

	DroneController();
	virtual ~DroneController();

	void connectToDrone();
	void setThrottleAndYaw(float throttle, float yaw);
	void setPitchAndRoll(float pitch, float roll);
	void emergencyPressed();
	void resetPressed();
	void changeCamera();

	Q_INVOKABLE void takeOff();
	Q_INVOKABLE void land();

signals:

	void connecting();
	void connected(QString firmwareVersion);
	void failedToConnect(QString failureReason);

private slots:

	void readFtp();
	void connectToFtp();
	void ftpConnectionTimeout();
	void handleConnected();

private:

	bool mConnectedToDrone;
	uint mConnectState;
	QString mFirmwareVersion;
	QFtp* m_pFtp;
	NavDataHandler* m_pNavDataHandler;
	AtCommand* m_pAtCommand;

};

#endif /* DRONECONTROLLER_H_ */
