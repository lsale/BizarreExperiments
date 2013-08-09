#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "Timer.hpp"
#include "NfcListener.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
	qmlRegisterType<Timer>("CustomTimer", 1, 0, "Timer");
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    if(!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()))) {
        // This is an abnormal situation! Something went wrong!
        // Add own code to recover here
        qWarning() << "Recovering from a failed connect()";
    }
    // initial load
    onSystemLanguageChanged();

    //NFC and invocation bit
    _invokeManager = new bb::system::InvokeManager();
    if (_invokeManager->startupMode() == bb::system::ApplicationStartupMode::InvokeApplication) {
    	_launchedByInvoke = true;
    }
    QDeclarativePropertyMap* propertyMap = new QDeclarativePropertyMap;
    propertyMap->insert("LaunchedByInvoke", QVariant(_launchedByInvoke ? "true" : "false"));

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    //register the property
    qml->setContextProperty("_propertyMap", propertyMap);

    // Create root object for the UI
    _root = qml->createRootObject<AbstractPane>();

    //NFC listeners
    initialiseListeners();

    // Set created root object as the application scene
    app->setScene(_root);
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("RunDonRun_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::initialiseListeners() {

	qDebug() << "App::initialiseNfcListener";
	_nfcListener = new NfcListener();

	QObject *obj = _root->findChild<QObject*>((const QString) "mainPane");

	QObject::connect(  _nfcListener,   SIGNAL(raceTagDetected(const QVariant &)),
			                    obj,     SLOT(raceTagDetected(const QVariant &)));

	QObject::connect(_invokeManager,   SIGNAL(invoked(const bb::system::InvokeRequest&)),
			           _nfcListener,     SLOT(receivedInvokeRequest(const bb::system::InvokeRequest&)));

	QObject::connect(          this,   SIGNAL(startListening()),
			           _nfcListener,     SLOT(startListening()));

	QObject::connect(          this,   SIGNAL(stopListening()),
			           _nfcListener,     SLOT(stopListening()));

	QObject::connect(           obj,   SIGNAL(startEmulatingTag(const QString &)),
			           _nfcListener,     SLOT(startTagEmulation(const QString &)));

	QObject::connect(           obj,   SIGNAL(updateEmulatingTag(const QString &)),
			           _nfcListener,     SLOT(updateTagEmulation(const QString &)));

	QObject::connect(           obj,   SIGNAL(stopEmulatingTag()),
			           _nfcListener,     SLOT(stopTagEmulation()));

	QObject::connect(  _nfcListener,   SIGNAL(tagEmulationSelectEvent()),
					            obj,     SLOT(tagEmulationSelectEvent()));

	QObject::connect(  _nfcListener,   SIGNAL(tagEmulationReadEvent()),
			                    obj,     SLOT(tagEmulationReadEvent()));

	QObject::connect(  _nfcListener,   SIGNAL(tagEmulationLeftEvent()),
			                    obj,     SLOT(tagEmulationLeftEvent()));

	emit startListening();
}

void ApplicationUI::exitApp() {
	emit stopListening();
	Application::exit(0);
}
