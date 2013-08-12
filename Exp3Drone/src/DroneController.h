/*
 * DroneController.h
 *
 *  Created on: 9 Aug 2013
 *      Author: doturner
 */

#ifndef DRONECONTROLLER_H_
#define DRONECONTROLLER_H_

#include <QObject>
#include <QUdpSocket>
#include <QMutex>
#include <QTimer>
#include <QFtp>

class DroneController : public QObject {
	Q_OBJECT
public:
	DroneController();
	virtual ~DroneController();

	Q_INVOKABLE void takeOff();
	Q_INVOKABLE void land();

signals:

	//void newDebugMessage(QString message);
	void droneConnected(QString version);

private slots:
	void droneConnectionTimeout();
	void newNavDataReady();
    void initTimer();
    void readDroneFtp();

private:

    QMutex mMutex;
	QUdpSocket* mUdpSocketNavData;
	QUdpSocket* mUdpSocketCommandData;
	QHostAddress receiver;
	QTimer* mInitTimer;
	QTimer* mLoaderTimer;
    QList<QByteArray> mDatagramList;

    int mCommandCount;
    bool mConnectedToDrone;
	bool mDroneResponded;
	bool mDroneBooting;
	QString mVersion;
	QFtp* mFtp;

	void lookUpHost();


	void initialize();


};

#endif /* DRONECONTROLLER_H_ */
