#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

#define RECORDING_PATH "/accounts/1000/shared/music/recording.wav"

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

    m_pDebugLabel = root->findChild<Label*>("debugLabel");

}

ApplicationUI::~ApplicationUI(){

	delete(m_pSoundManager);
}


void ApplicationUI::loadSound(){

	QString recordingPath(RECORDING_PATH);

	if (m_pSoundManager->load(recordingPath)){

		QString debug("Loaded: ");
		debug.append(recordingPath);
		setDebugText(debug);
	} else {
		setDebugText("Failed to load recording");
	}
}

/**
 * Plays the sound
 */
void ApplicationUI::playSound(){

	//Play the sound
	QString recordingPath(RECORDING_PATH);
	m_pSoundManager->play(recordingPath, 1.0f, 1.0f);
}

/**
 * Stops the sound
 */
void ApplicationUI::stopSound(){

	QString recordingPath(RECORDING_PATH);
	m_pSoundManager->stop(recordingPath);
}

void ApplicationUI::setPitch(float newPitch){

	QString recordingPath(RECORDING_PATH);
	m_pSoundManager->setPitch(recordingPath,newPitch);

}

void ApplicationUI::playTone(){

	if (!m_pSoundManager->playSquareTone()){
		setDebugText("Failed to play tone");
	}
}

void ApplicationUI::setDebugText(QString debugText){

	m_pDebugLabel->setText(debugText);
}

