/*
 * GamePadListener.cpp
 *
 *  Created on: 12 Aug 2013
 *      Author: doturner
 */

#include "GamePadListener.h"
#include <bps/bps.h>
#include <errno.h>

#include <QDebug>

// This macro provides error checking for all calls to libscreen APIs.
static int rc;
#define SCREEN_API(x, y) rc = x; \
    if (rc) fprintf(stderr, "\n%s in %s: %d", y, __FUNCTION__, errno)


// Each button type corresponds to a set of texture coordinates from those defined above.
typedef enum ButtonType_t {
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	DIGITAL_TRIGGER,
	ANALOG_TRIGGER,
	BUTTON
} ButtonType;

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

// Controller information.
static int MAX_CONTROLLERS = 2;
//static int MAX_BUTTONS = 16;
static GameController _controllers[2];

GamePadListener::GamePadListener(screen_context_t* screen_context) : m_pScreenContext(screen_context){

	//Subscribe to screen events
	subscribe(screen_get_domain());
	bps_initialize();
	screen_request_events(*m_pScreenContext);

	discoverControllers();

	qDebug() << "[GamePadListener] initialized";


}

GamePadListener::~GamePadListener() {

	screen_stop_events(*m_pScreenContext);
	unsubscribe(screen_get_domain());
	bps_shutdown();
}

void GamePadListener::event(bps_event_t* event) {

	//qDebug() << "[GamePadListener] event received. Event code: " << bps_event_get_code(event);

	int eventType;
	screen_event_t screen_event = screen_event_get_event(event);
	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
			&eventType);

	//qDebug() << "[GamePadListener] Event type: " << eventType;

	switch (eventType) {
	case SCREEN_EVENT_GAMEPAD:
	case SCREEN_EVENT_JOYSTICK:
		//qDebug() << "Got GamePad or Joystick event!";
		break;
	}

}

void GamePadListener::discoverControllers()
{
    // Get an array of all available devices.
    int deviceCount;
    SCREEN_API(screen_get_context_property_iv(*m_pScreenContext, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount), "SCREEN_PROPERTY_DEVICE_COUNT");
    screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));
    SCREEN_API(screen_get_context_property_pv(*m_pScreenContext, SCREEN_PROPERTY_DEVICES, (void**)devices), "SCREEN_PROPERTY_DEVICES");

    // Scan the list for gamepad and joystick devices.
    int i;
    int controllerIndex = 0;
    for (i = 0; i < deviceCount; i++) {
        int type;
        SCREEN_API(screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type), "SCREEN_PROPERTY_TYPE");

        if (!rc && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)) {

        	// Assign this device to control Player 1 or Player 2.
            GameController* controller = &_controllers[controllerIndex];
            controller->handle = devices[i];
            loadController(controller);

            // We'll just use the first compatible devices we find.
            controllerIndex++;
            if (controllerIndex == MAX_CONTROLLERS) {
                break;
            }
        }
    }

    free(devices);
}

void GamePadListener::loadController(GameController* controller)
{
    // Query libscreen for information about this device.
    SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_TYPE, &controller->type), "SCREEN_PROPERTY_TYPE");
    SCREEN_API(screen_get_device_property_cv(controller->handle, SCREEN_PROPERTY_ID_STRING, sizeof(controller->id), controller->id), "SCREEN_PROPERTY_ID_STRING");
    SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTON_COUNT, &controller->buttonCount), "SCREEN_PROPERTY_BUTTON_COUNT");

    // Check for the existence of analog sticks.
    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0)) {
    	++controller->analogCount;
    }

    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1)) {
    	++controller->analogCount;
    }

    if (controller->type == SCREEN_EVENT_GAMEPAD) {
        sprintf(controller->deviceString, "Gamepad device ID: %s", controller->id);
    } else {
        sprintf(controller->deviceString, "Joystick device: %s", controller->id);
    }

    qDebug() << "[GamePadListener] Loaded controller: " << controller->deviceString;
}
