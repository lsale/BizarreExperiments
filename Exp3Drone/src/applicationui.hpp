#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include "GamePadHandler.h"
#include "drone/DroneController.h"
#include "VideoDecoder.h"

#include <QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Label>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/ForeignWindowControl>

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

using namespace bb::cascades;

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();
private slots:
	void updateDroneStatusLabel(QString statusText);
	void updateGamePadStatusLabel(QString statusText);

	void handleGamePadConnected(int playerNo);
	void handleGamePadDisconnected(int playerNo);
	void handleGamePadButtonStateChanged(int playerNo,int newState);
	void handleGamePadAnalog0StateChanged(int playerNo,int newXState,int newYState);
	void handleGamePadAnalog1StateChanged(int playerNo,int newXState,int newYState);


	void handleDroneConnecting();
	void handleDroneConnected(QString firmwareVersion);
	void handleDroneConnectionFailed(QString failureReason);

private:

	//UI elements
	AbstractPane* m_pRootPane;
	Label* m_pDroneStatusLabel;
	Label* m_pGamePadStatusLabel;
	ActivityIndicator* m_pDroneActivityIndicator;
	ActivityIndicator* m_pGamePadActivityIndicator;
	ForeignWindowControl* m_pVideoWindowControl;

	//Back end logic
	GamePadHandler* m_pGamePadHandler;
	GameController* m_pGameController;
	DroneController* m_pDroneController;
	VideoDecoder* m_pVideoDecoder;

	bool isButtonPressed(int buttonState, int button);
};

#endif /* ApplicationUI_HPP_ */
