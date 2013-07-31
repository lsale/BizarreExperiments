#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include "soundmanager.h"

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

    Q_INVOKABLE void playTiger(float pitch);
    Q_INVOKABLE void stopTiger();
    Q_INVOKABLE void changeTigerPitch(float newPitch);

private slots:
    void onSystemLanguageChanged();
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

    SoundManager* m_pSoundManager;
    ALint m_tigerSourceId;
};

#endif /* ApplicationUI_HPP_ */
