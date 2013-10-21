#include "applicationui.hpp"

#include "Timer.cpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>
#include <pthread.h>
#include <errno.h>

using namespace bb::cascades;

#define DRUM_PATH "/accounts/1000/shared/music/drum.wav"

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app)
{

	//Stop the screen from timing out
	app->mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);

	//Read the current scheduling parameters
	sched_param schedParam;
	int policy;
	int getParamsResult = pthread_getschedparam(pthread_self(), &policy, &schedParam);

	if (getParamsResult == EOK)
	{
		qDebug() << getSchedulingPolicyDescription(policy);
	}

	//Change the thread scheduling policy to FIFO
	int setScheduleResult = pthread_setschedparam(pthread_self(), SCHED_FIFO, &schedParam);
	if (setScheduleResult == EOK)
	{
		qDebug() << "Set scheduler to FIFO";
	}
	else
	{
		qDebug() << "Error setting scheduling policy to FIFO. Error: " << setScheduleResult;
	}

	//Change the thread priority to the maximum permissible
	int priorityResult = pthread_setschedprio(pthread_self(), 63);

	if (priorityResult == EOK)
	{
		qDebug() << "Successfully set new thread priority";
	}
	else
	{
		qDebug() << "Failed to set new thread priority. Result: " << priorityResult;
	}

	//Initialise our sound manager, which will play our sounds for us
	m_pSoundManager = new SoundManager();

	//Load the bass note
	//mBassPath = QDir::currentPath();
	//mBassPath.append("/app/native/assets/bass.wav");
	//m_pSoundManager->load(mBassPath);

	//Create a timer for the drum
	/*m_pDrumTimer = new QTimer();
	m_pDrumTimer->setInterval(500);

	//Connect the timeout to playing the drum
	connect(m_pDrumTimer, SIGNAL(timeout()), this, SLOT(playDrum()));
*/
	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// Expose the app object to the qml
	qml->setContextProperty("app", this);

	// Create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	//Set the recording path of the drum
	QString recordingUrl("file://");
	recordingUrl.append(DRUM_PATH);
	root->setProperty("drumUrl", recordingUrl);

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

void ApplicationUI::loadDrum(){

	if (m_pSoundManager->load(DRUM_PATH)){

		QString debug("Loaded: ");
		debug.append(DRUM_PATH);
		qDebug() << "Loaded: " << DRUM_PATH;
	} else {
		qDebug() << "Failed to load recording";
	}
}

void ApplicationUI::startDrum()
{
	qDebug() << "[ApplicationUI] startDrum - start";
	m_pDrumTimer->start();
	qDebug() << "[ApplicationUI] startDrum - end";
}

void ApplicationUI::stopDrum()
{
	qDebug() << "[ApplicationUI] stopDrum - start";
	m_pDrumTimer->stop();

	//Stop the drum from playing
	m_pSoundManager->stop(DRUM_PATH);
	qDebug() << "[ApplicationUI] stopDrum - end";
}

void ApplicationUI::playDrum()
{
	qDebug() << "[ApplicationUI] playDrum - start";
	m_pSoundManager->play(DRUM_PATH, 1.0, 1.0);
	qDebug() << "[ApplicationUI] playDrum - end";
}

QString ApplicationUI::getSchedulingPolicyDescription(const int policy) {

	QString policyName;

	switch (policy) {
	case SCHED_FIFO:
		policyName = "FIFO";
		break;
	case SCHED_RR:
		policyName = "RR";
		break;
	default:
		policyName = policy;
		break;
	}

	return policyName;
}
