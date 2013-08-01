#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/AbstractPane>


#include "soundmanager.h"
#include "bluetooth/BluetoothHandler.hpp"

namespace bb {
namespace cascades {
class Application;
class LocaleHandler;
}
}

class QTranslator;

class ApplicationUI: public QObject {
Q_OBJECT
public:
	ApplicationUI(bb::cascades::Application *app);
	virtual ~ApplicationUI();

	Q_INVOKABLE
	void monitorHeartRate(QString device_addr, QString device_name);Q_INVOKABLE
	void playTiger(float pitch);Q_INVOKABLE
	void stopTiger();Q_INVOKABLE
	bool isTigerPlaying();Q_INVOKABLE
	void changeTigerPitch(float newPitch);Q_INVOKABLE
	void connectToHRMonitor();

public slots:
	void logHeartRate(const QVariant &message);
	void finishedReceiving();

private slots:
	void onSystemLanguageChanged();
private:
	QTranslator* m_pTranslator;
	bb::cascades::LocaleHandler* m_pLocaleHandler;

	//Heart rate stuff
	BluetoothHandler* m_pBluetoothHandler;
	QFuture<void> *_future;
	QFutureWatcher<void> *_watcher;
	QMutex _mutex;

	//Sound stuff
	SoundManager* m_pSoundManager;
	ALint m_tigerSourceId;

	//UI
	bb::cascades::AbstractPane* m_pRoot;
};

#endif /* ApplicationUI_HPP_ */
