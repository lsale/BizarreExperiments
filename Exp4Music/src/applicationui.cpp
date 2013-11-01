#include "applicationui.hpp"

#include "Timer.cpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>
#include <pthread.h>
#include <errno.h>

using namespace bb::cascades;

#define SAMPLE_PATH "/accounts/1000/shared/music/sample.wav"

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

	//Create a timer for the sample
	m_pSampleTimer = new QTimer();
	m_pSampleTimer->setInterval(500);

	//Connect the timeout to playing the sample
	connect(m_pSampleTimer, SIGNAL(timeout()), this, SLOT(playSample()));

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main-debug.qml").parent(this);

	// Expose the app object to the qml
	qml->setContextProperty("app", this);

	// Create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	//Set the recording path of the sample
	QString recordingUrl("file://");
	recordingUrl.append(SAMPLE_PATH);
	root->setProperty("sampleUrl", recordingUrl);

	// Set created root object as the application scene
	app->setScene(root);
}

ApplicationUI::~ApplicationUI()
{
	delete m_pSoundManager;
	delete m_pSampleTimer;
}

void ApplicationUI::playGuitar(float pitch, float gain)
{
	m_pSoundManager->playGuitar(pitch, gain);
}

void ApplicationUI::loadSample(){

	qDebug() << "[ApplicationUI] loadSample - start";
	m_pSoundManager->loadSample(SAMPLE_PATH);
	qDebug() << "[ApplicationUI] loadSample - end";
}

/*void ApplicationUI::playSample(bool shouldLoop)
{
	qDebug() << "[ApplicationUI] startSample - start";

	m_pSoundManager->playSample();

	if (shouldLoop)
	{
		//if the timer is already running stop it
		if (m_pSampleTimer->isActive()){
			m_pSampleTimer->stop();
		}
		//if the sample is currently being played, stop it
		m_pSoundManager->stop()
	}

	m_pSampleTimer->start();
	qDebug() << "[ApplicationUI] startSample - end";
}*/

void ApplicationUI::stopSample()
{
	qDebug() << "[ApplicationUI] stopSample - start";
	m_pSampleTimer->stop();

	//Stop the sample from playing
	//m_pSoundManager->stop(SAMPLE_PATH);
	qDebug() << "[ApplicationUI] stopSample - end";
}

/*void ApplicationUI::playSample()
{
	qDebug() << "[ApplicationUI] playSample - start";

	qDebug() << "[ApplicationUI] playSample - end";
}*/

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
