#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <bb/cascades/AbstractPane>

#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeRequest.hpp>
#include <QtNfcSubset/qndefmessage.h>

#include <bps/navigator.h>
#include <bps/navigator_invoke.h>

#include "NfcListener.hpp"


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
    virtual ~ApplicationUI() { }
    Q_INVOKABLE void exitApp();

private slots:
    void onSystemLanguageChanged();
private:
    //Create the NFC listeners and invokeManager
    bb::system::InvokeManager *_invokeManager;
    bool _launchedByInvoke;
    NfcListener *_nfcListener;
    void initialiseListeners();
    bb::cascades::AbstractPane *_root;

    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

signals:
    void startListening();
    void stopListening();
};

#endif /* ApplicationUI_HPP_ */
