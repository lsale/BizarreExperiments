#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>

#include <bps/screen.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{

	qmlRegisterType<DroneController>();
	m_pDroneController = new DroneController();

	// Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("droneController", m_pDroneController);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    //Get the screen context of the main window
    screen_window_t screen_window = (screen_window_t) app->mainWindow()->handle();
    screen_context_t screen_context;
    screen_get_window_property_pv(screen_window, SCREEN_PROPERTY_CONTEXT, (void **)&screen_context);

    m_pGamePadListener = new GamePadListener(&screen_context);
}

ApplicationUI::~ApplicationUI(){

	delete(m_pDroneController);
	delete(m_pGamePadListener);
}
