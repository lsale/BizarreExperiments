#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

#define RECORDING_PATH "/accounts/1000/shared/music/recording.wav"

static ALuint bufferId;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{

	//Initialise our sound manager, which will play our sounds for us
	m_pSoundManager = new SoundManager();


	// Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    QString recordingUrl("file://");
    recordingUrl.append(RECORDING_PATH);
    root->setProperty("recordingUrl", recordingUrl);

    // Set created root object as the application scene
    app->setScene(root);
}

ApplicationUI::~ApplicationUI(){

	delete(m_pSoundManager);
}

/**
 * Plays the sound
 */
void ApplicationUI::playSound(){

	//Load the sound into OpenAL
	QString recordingPath(RECORDING_PATH);
	bufferId = m_pSoundManager->load(recordingPath);

	//Play the sound
	m_pSoundManager->play(bufferId, 1.0f, 1.0f);
}

/**
 * Stops the sound
 */
void ApplicationUI::stopSound(){

}

