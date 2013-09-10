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

    Q_INVOKABLE void playBass(float pitch);
    Q_INVOKABLE void loadDrum();
    Q_INVOKABLE void startDrum();
    Q_INVOKABLE void stopDrum();

private slots:
	void playDrum();
private:

	SoundManager* m_pSoundManager;
	QString mBassPath;
	QTimer* m_pDrumTimer;

	QString getSchedulingPolicyDescription(const int policy);
};

#endif /* ApplicationUI_HPP_ */
