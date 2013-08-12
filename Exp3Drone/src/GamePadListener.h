/*
 * GamePadListener.h
 *
 * Listens for GamePad events
 *
 *  Created on: 12 Aug 2013
 *      Author: doturner
 */

#ifndef GAMEPADLISTENER_H_
#define GAMEPADLISTENER_H_

#include <QObject>
#include <bb/AbstractBpsEventHandler>

#include <bps/screen.h>

struct GameController_t;

class GamePadListener: public QObject, public bb::AbstractBpsEventHandler {
	Q_OBJECT

public:
	GamePadListener(screen_context_t* screen_context);
	virtual ~GamePadListener();
	virtual void event(bps_event_t *event);

signals:

	//Emitted when a GamePad is connected to the device
	void gamePadConnected(int playerNo);

	//Emmited when a GamePad is disconnected from the device
	void gamePadDisconnected(int playerNo);

private:

	screen_context_t* m_pScreenContext;

	void discoverControllers();
	void loadController(GameController_t* controller);
};

#endif /* GAMEPADLISTENER_H_ */
