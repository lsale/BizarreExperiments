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

private:
	SoundManager* m_pSoundManager;
	QString mBassPath;
};

#endif /* ApplicationUI_HPP_ */
