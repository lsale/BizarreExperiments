#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include "soundmanager.h"

#include <QObject>
#include <QTimer>

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();

    Q_INVOKABLE void playGuitar(float pitch, float gain);
    Q_INVOKABLE void loadSample();
    Q_INVOKABLE void stopSample();
    Q_INVOKABLE void startSampleLoop();

public slots:
	Q_INVOKABLE void playSample(); //This is a slot because it's invoked by the timer when looping

private:

	SoundManager* m_pSoundManager;
	QString mBassPath;
	QTimer* m_pSampleTimer;

	QString getSchedulingPolicyDescription(const int policy);
};

#endif /* ApplicationUI_HPP_ */
