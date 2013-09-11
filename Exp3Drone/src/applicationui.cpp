#include "applicationui.hpp"

#include <QDebug>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>

#include <bps/screen.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app)
{
	qDebug() << "[ApplicationUI] Constructor - start";

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument* pQmlDocument = QmlDocument::create("asset:///main.qml").parent(this);

	// Create root object for the UI
	m_pRootPane = pQmlDocument->createRootObject<AbstractPane>();

	// Obtain the UI elements
	m_pDroneStatusLabel = m_pRootPane->findChild<Label*>("droneStatusLabel");
	m_pGamePadStatusLabel = m_pRootPane->findChild<Label*>("gamePadStatusLabel");
	m_pDroneActivityIndicator = m_pRootPane->findChild<ActivityIndicator*>("droneActivityIndicator");
	m_pGamePadActivityIndicator = m_pRootPane->findChild<ActivityIndicator*>("gamePadActivityIndicator");
	m_pVideoWindowControl = m_pRootPane->findChild<ForeignWindowControl*>("videoWindowControl");

	//Initialise the video decoder by passing it the details of our foreign window control
	m_pVideoDecoder = new VideoDecoder(m_pVideoWindowControl->windowGroup(), m_pVideoWindowControl->windowId());

	// Set created root object as the application scene
	app->setScene(m_pRootPane);

	//Get the screen context of the main window
	screen_window_t screen_window = (screen_window_t) app->mainWindow()->handle();
	screen_context_t screen_context;
	screen_get_window_property_pv(screen_window, SCREEN_PROPERTY_CONTEXT, (void **) &screen_context);

	// Create the gamepad handler and start listening for gamepad events
	m_pGamePadHandler = new GamePadHandler(&screen_context);

	bool success1 = connect(m_pGamePadHandler, SIGNAL(gamePadConnected(int)), this, SLOT(handleGamePadConnected(int)));
	bool success2 = connect(m_pGamePadHandler, SIGNAL(gamePadDisconnected(int)), this, SLOT(handleGamePadDisconnected(int)));
	bool success3 = connect(m_pGamePadHandler, SIGNAL(buttonStateChanged(int,int)), this, SLOT(handleGamePadButtonStateChanged(int,int)));
	bool success4 = connect(m_pGamePadHandler, SIGNAL(analog0StateChanged(int,int,int)), this, SLOT(handleGamePadAnalog0StateChanged(int,int,int)));
	bool success5 = connect(m_pGamePadHandler, SIGNAL(analog1StateChanged(int,int,int)), this, SLOT(handleGamePadAnalog1StateChanged(int,int,int)));

	if (success1 && success2 && success3 && success4 && success5)
	{
		m_pGamePadHandler->start();
		updateGamePadStatusLabel("Searching for Bluetooth GamePad...");
		m_pGamePadActivityIndicator->start();
	}
	else
	{
		qDebug() << "[ApplicationUI] Failed to connect GamePadHandler to ApplicationUI";
	}

	// Now create the drone controller and start listening for drone events
	m_pDroneController = new DroneController();
	bool success6 = connect(m_pDroneController, SIGNAL(connecting()), this, SLOT(handleDroneConnecting()));
	bool success7 = connect(m_pDroneController, SIGNAL(connected(QString)), this, SLOT(handleDroneConnected(QString)));
	bool success8 = connect(m_pDroneController, SIGNAL(failedToConnect(QString)), this, SLOT(handleDroneConnectionFailed(QString)));

	if (success6 && success7 && success8)
	{
		m_pDroneController->connectToDrone();
	}

	//Register the drone controller for use within QML
	pQmlDocument->setContextProperty("drone", m_pDroneController);

	qDebug() << "[ApplicationUI] Constructor - end";
}

ApplicationUI::~ApplicationUI()
{
	qDebug() << "[ApplicationUI] Destructor - start";

	m_pVideoDecoder->stop();
	m_pVideoDecoder->wait();
	delete m_pVideoDecoder;
	m_pVideoDecoder = 0;

	delete m_pDroneController;
	m_pDroneController = 0;

	m_pGamePadHandler->stop();
	m_pGamePadHandler->wait();

	qDebug() << "[ApplicationUI] Destructor - waiting for gamepad handler thread to end";

	delete m_pGamePadHandler;
	m_pGamePadHandler = 0;

	qDebug() << "[ApplicationUI] Destructor - end";
}

void ApplicationUI::handleGamePadConnected(int playerNo)
{
	qDebug() << "[ApplicationUI] handleGamePadConnected - Device connected, player no: " << playerNo;
	updateGamePadStatusLabel("GamePad connected");
	m_pGamePadActivityIndicator->stop();

}

void ApplicationUI::handleGamePadDisconnected(int playerNo)
{
	qDebug() << "[ApplicationUI] handleGamePadDisconnected - Device disconnected, player no: " << playerNo;
	updateGamePadStatusLabel("Searching for Bluetooth GamePad...");
	m_pGamePadActivityIndicator->start();
}

void ApplicationUI::handleGamePadButtonStateChanged(int playerNo, int newState)
{
	Q_UNUSED(playerNo);

	qDebug() << "[ApplicationUI] Button state changed to: " << newState;

	if (isButtonPressed(newState, SCREEN_R1_GAME_BUTTON)) m_pDroneController->takeOff();
	if (isButtonPressed(newState, SCREEN_L1_GAME_BUTTON)) m_pDroneController->land();
	if (isButtonPressed(newState, SCREEN_Y_GAME_BUTTON)) m_pDroneController->emergencyPressed();
	if (isButtonPressed(newState, SCREEN_B_GAME_BUTTON)) m_pDroneController->resetPressed();
	if (isButtonPressed(newState, SCREEN_A_GAME_BUTTON)) QMetaObject::invokeMethod(m_pRootPane, "playHorn");
	if (isButtonPressed(newState, SCREEN_X_GAME_BUTTON)) m_pDroneController->changeCamera();

}

void ApplicationUI::handleGamePadAnalog0StateChanged(int playerNo, int newXState, int newYState)
{
	Q_UNUSED(playerNo);

	qDebug() << "[ApplicationUI] Analog0 state changed, x: " << newXState << ", y: " << newYState;

	//Convert to range -1 to 1
	float throttle = (float) -newYState / 127;
	float yaw = (float) newXState / 128;

	qDebug() << "[ApplicationUI] Setting throttle to: " << throttle << ", yaw to: " << yaw;
	m_pDroneController->setThrottleAndYaw(throttle, yaw);
}

void ApplicationUI::handleGamePadAnalog1StateChanged(int playerNo, int newXState, int newYState)
{
	Q_UNUSED(playerNo);

	qDebug() << "[ApplicationUI] Analog1 state changed, x: " << newXState << ", y: " << newYState;

	//Convert to range -1 to 1
	float pitch = (float) newYState / 127;
	float roll = (float) newXState / 128;

	qDebug() << "[ApplicationUI] Setting pitch to: " << pitch << ", roll to: " << roll;
	m_pDroneController->setPitchAndRoll(pitch, roll);
}

void ApplicationUI::handleDroneConnecting()
{
	m_pDroneActivityIndicator->start();
	updateDroneStatusLabel("Connecting to drone...");
}

void ApplicationUI::handleDroneConnected(QString firmwareVersion)
{
	updateDroneStatusLabel(QString("Connected to drone. Firmware %1").arg(firmwareVersion));
	m_pDroneActivityIndicator->stop();

	//Start displaying the video
	m_pVideoDecoder->start();
}

void ApplicationUI::handleDroneConnectionFailed(QString failureReason)
{
	updateDroneStatusLabel(QString("Failed: %1").arg(failureReason));
	m_pDroneActivityIndicator->stop();
}

void ApplicationUI::updateDroneStatusLabel(QString statusText)
{
	if (m_pDroneStatusLabel)
	{
		m_pDroneStatusLabel->setText(statusText);
	}
}

void ApplicationUI::updateGamePadStatusLabel(QString statusText)
{
	if (m_pGamePadStatusLabel)
	{
		m_pGamePadStatusLabel->setText(statusText);
	}
}

bool ApplicationUI::isButtonPressed(int buttonState, int button)
{
	return ((buttonState & button) == button);
}
