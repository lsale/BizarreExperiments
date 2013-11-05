#include "applicationui.hpp"

#include "Timer.cpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>
#include <pthread.h>
#include <errno.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app)
{

	//Stop the screen from timing out
	app->mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);

	//Initialise our sound manager, which will play our sounds for us
	m_pSoundManager = new SoundManager();

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// Expose the app object to the qml
	qml->setContextProperty("app", this);

	// Create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	// Set created root object as the application scene
	app->setScene(root);
}

ApplicationUI::~ApplicationUI()
{
	delete m_pSoundManager;
}

void ApplicationUI::playGuitar(float pitch, float gain)
{
	m_pSoundManager->playGuitar(pitch, gain);
}
