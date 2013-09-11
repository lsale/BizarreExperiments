/*
 * GamePadHandler.h
 *
 *  Created on: 27 Aug 2013
 *      Author: doturner
 */

#ifndef GAMEPADHANDLER_H_
#define GAMEPADHANDLER_H_

#include <QThread>

#include <bps/screen.h>

// Structure representing a game controller.
typedef struct GameController_t {
    // Static device info.
    screen_device_t handle;
    int type;
    int analogCount;
    int buttonCount;
    char id[64];

    // Current state.
    int buttons;
    int analog0[3];
    int analog1[3];

    // Text to display to the user about this controller.
    char deviceString[256];
    char buttonsString[128];
    char analog0String[128];
    char analog1String[128];
} GameController;


class GamePadHandler: public QThread
{
	Q_OBJECT

public:
	GamePadHandler(screen_context_t* screen_context);
	virtual ~GamePadHandler();

	void run();
	void stop();
	GameController* getController(int playerNo);

signals:
	void gamePadConnected(int playerNo);
	void gamePadDisconnected(int playerNo);
	void buttonStateChanged(int playerNo, int newState);
	void analog0StateChanged(int playerNo, int newXState, int newYState);
	void analog1StateChanged(int playerNo, int newXState, int newYState);

private:
	bool shouldRun;
	screen_context_t mScreenContext;
	GameController mControllers[2];

	void initController(GameController* controller, int player);
	void discoverControllers();
	void loadController(GameController* controller);
	void handleEvents();
	void handleScreenEvent(bps_event_t *event);
};

#endif /* GAMEPADHANDLER_H_ */
