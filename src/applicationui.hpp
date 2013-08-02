#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include "soundmanager.h"

#include <QObject>

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();

    Q_INVOKABLE void playSound();
    Q_INVOKABLE void stopSound();

private slots:

private:

	SoundManager* m_pSoundManager;

};

#endif /* ApplicationUI_HPP_ */
