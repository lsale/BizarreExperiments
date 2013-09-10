#ifndef NAVDATAHANDLER_H
#define NAVDATAHANDLER_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QUdpSocket>

class QTimer;

class NavDataHandler : public QThread
{
    Q_OBJECT
public:
    NavDataHandler(QObject *parent = 0);
    ~NavDataHandler();

protected:
    void run();

signals:
    void updateARDroneState(uint state);
    void updateBatteryLevel(uint level);
    void updateEmergencyState(bool on);
    void sendInitSequence(int step);

private slots:
    void newNavDataReady();
    void sendReadySignal();

private:
    void initialize();

private:
    QMutex mMutex;
    QWaitCondition mCondition;
    bool mAbort;
    QUdpSocket *mUdpSocketNavData;
    QList<QByteArray> mDatagramList;

    QTimer *mInitTimer;
    bool mDroneResponded;
    bool mDroneBooting;
};

#endif // NAVDATAHANDLER_H
