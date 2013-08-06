#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include "soundmanager.h"

#include <QObject>
#include <bb/cascades/Label>


namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

using namespace bb::cascades;

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();

    Q_INVOKABLE void loadSound();
    Q_INVOKABLE void playSound();
    Q_INVOKABLE void stopSound();
    Q_INVOKABLE void setPitch(float newPitch);
    Q_INVOKABLE void playTone();

private slots:

private:

	SoundManager* m_pSoundManager;
	Label* m_pDebugLabel;

	void setDebugText(QString debugText);

};

#endif /* ApplicationUI_HPP_ */
