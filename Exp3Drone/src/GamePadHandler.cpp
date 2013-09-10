/*
 * GamePadHandler.cpp
 *
 *  Created on: 27 Aug 2013
 *      Author: doturner
 */

#include "GamePadHandler.h"

#include <QDebug>

#include <screen/screen.h>
#include <errno.h>

// This macro provides error checking for all calls to libscreen APIs.
static int rc;
#define SCREEN_API(x, y) rc = x; \
    if (rc) fprintf(stderr, "\n%s in %s: %d", y, __FUNCTION__, errno)

#define MAX_CONTROLLERS 2

GamePadHandler::GamePadHandler(screen_context_t* screen_context) :
		shouldRun(true), mScreenContext(*screen_context)
{
	qDebug() << "[GamePadHandler] Constructor start";

	qDebug() << "[GamePadHandler] Constructor end";
}

GamePadHandler::~GamePadHandler()
{
	qDebug() << "[GamePadHandler] Destructor start";

	qDebug() << "[GamePadHandler] Destructor end";
}

void GamePadHandler::run()
{
	qDebug() << "[GamePadHandler] run() - start";

	shouldRun = true;

	// Initialize BPS library.
	bps_initialize();

	// Initialize the controllers
	int i;
	for (i = 0; i < MAX_CONTROLLERS; ++i)
	{
		initController(&mControllers[i], i);
	}

	// Request screen events
	screen_request_events(mScreenContext);

	// Look for attached gamepads
	discoverControllers();

	//Main event loop
	while (shouldRun)
	{
		//do some stuff
		//qDebug() << "[GamePadHandler] Polling for gamepad events";
		//sleep(1);

		handleEvents();
		//update();
		//render();
	}

	// Clean up resources and shut everything down.

	// Stop requesting events from libscreen.
	screen_stop_events(mScreenContext);
	// Shut down BPS library for this process.
	bps_shutdown();

	qDebug() << "[GamePadHandler] run() - end";

}

void GamePadHandler::initController(GameController* controller, int player)
{
	// Initialize controller values.
	controller->handle = 0;
	controller->type = 0;
	controller->analogCount = 0;
	controller->buttonCount = 0;
	controller->buttons = 0;
	controller->analog0[0] = controller->analog0[1] = controller->analog0[2] = 0;
	controller->analog1[0] = controller->analog1[1] = controller->analog1[2] = 0;
	sprintf(controller->deviceString, "Player %d: No device detected.", player + 1);
}

void GamePadHandler::discoverControllers()
{
	// Get an array of all available devices.
	int deviceCount;
	SCREEN_API(screen_get_context_property_iv(mScreenContext, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount), "SCREEN_PROPERTY_DEVICE_COUNT");
	screen_device_t* devices = (screen_device_t*) calloc(deviceCount, sizeof(screen_device_t));
	SCREEN_API(screen_get_context_property_pv(mScreenContext, SCREEN_PROPERTY_DEVICES, (void** )devices), "SCREEN_PROPERTY_DEVICES");

	// Scan the list for gamepad and joystick devices.
	int i;
	int controllerIndex = 0;
	for (i = 0; i < deviceCount; i++)
	{
		int type;
		SCREEN_API(screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type), "SCREEN_PROPERTY_TYPE");

		if (!rc && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK))
		{

			// Assign this device to control Player 1 or Player 2.
			GameController* controller = &mControllers[controllerIndex];
			controller->handle = devices[i];
			loadController(controller);
			qDebug() << "[GamePadHandler] discoverControllers - Device already connected, index: " << controllerIndex << " name: " << controller->deviceString;
			emit gamePadConnected(controllerIndex);
			// We'll just use the first compatible devices we find.
			controllerIndex++;
			if (controllerIndex == MAX_CONTROLLERS)
			{
				break;
			}
		}
	}

	free(devices);
}

void GamePadHandler::loadController(GameController* controller)
{
	// Query libscreen for information about this device.
	SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_TYPE, &controller->type), "SCREEN_PROPERTY_TYPE");
	SCREEN_API(screen_get_device_property_cv(controller->handle, SCREEN_PROPERTY_ID_STRING, sizeof(controller->id), controller->id),
			"SCREEN_PROPERTY_ID_STRING");
	SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTON_COUNT, &controller->buttonCount), "SCREEN_PROPERTY_BUTTON_COUNT");

	// Check for the existence of analog sticks.
	if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0))
	{
		++controller->analogCount;
	}

	if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1))
	{
		++controller->analogCount;
	}

	if (controller->type == SCREEN_EVENT_GAMEPAD)
	{
		sprintf(controller->deviceString, "Gamepad device ID: %s", controller->id);
	}
	else
	{
		sprintf(controller->deviceString, "Joystick device: %s", controller->id);
	}

	qDebug() << "[GamePadHandler] loadController() - Loaded device ID: " << controller->id;
}

void GamePadHandler::handleEvents()
{
	// Get the first event in the queue.
	bps_event_t *event = NULL;
	if (BPS_SUCCESS != bps_get_event(&event, 0))
	{
		qDebug() << "[GamePadHandler] handleEvents() - bps_get_event() failed";
		return;
	}

	// Handle all events in the queue.
	// If we don't do this in a loop, we'll only handle one event per frame.
	// If many events are triggered quickly, e.g. by spinning the analog sticks,
	// the queue will grow and the user will see the analog sticks lag.
	while (event)
	{

		handleScreenEvent(event);

		if (BPS_SUCCESS != bps_get_event(&event, 0))
		{
			qDebug() << "[GamePadHandler] handleEvents() - bps_get_event() failed";
			return;
		}
	}
}

void GamePadHandler::handleScreenEvent(bps_event_t *event)
{
	int eventType;

	screen_event_t screen_event = screen_event_get_event(event);
	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &eventType);

	switch (eventType)
	{
	case SCREEN_EVENT_GAMEPAD:
	case SCREEN_EVENT_JOYSTICK:
	{
		//qDebug() << "[GamePadHandler] handleScreenEvent() - Got gamepad event";

		// Determine which controller this is.
		screen_device_t device;
		SCREEN_API(screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void** )&device), "SCREEN_PROPERTY_DEVICE");

		GameController* controller = NULL;
		int i;
		for (i = 0; i < MAX_CONTROLLERS; ++i)
		{
			if (device == mControllers[i].handle)
			{
				controller = &mControllers[i];
				break;
			}
		}

		if (!controller)
		{
			break;
		}

		// Store the controller's new state.
		int oldButtonState = controller->buttons;

		SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &controller->buttons), "SCREEN_PROPERTY_BUTTONS");

		if (oldButtonState != controller->buttons){
			qDebug() << "[GamePadHandler] button state changed to: " << controller->buttons;
			emit buttonStateChanged(i, controller->buttons);
		}

		if (controller->analogCount > 0)
		{
			int oldAnalog0XValue = controller->analog0[0];
			int oldAnalog0YValue = controller->analog0[1];

			SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG0, controller->analog0), "SCREEN_PROPERTY_ANALOG0");

			if (oldAnalog0XValue != controller->analog0[0] || oldAnalog0YValue != controller->analog0[1]){
				emit analog0StateChanged(i, controller->analog0[0], controller->analog0[1]);
			}
		}

		if (controller->analogCount == 2)
		{
			int oldAnalog1XValue = controller->analog1[0];
			int oldAnalog1YValue = controller->analog1[1];

			SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG1, controller->analog1), "SCREEN_PROPERTY_ANALOG1");

			if (oldAnalog1XValue != controller->analog1[0] || oldAnalog1YValue != controller->analog1[1]){
				emit analog1StateChanged(i, controller->analog1[0], controller->analog1[1]);
			}
		}
		break;
	}
	case SCREEN_EVENT_DEVICE:
	{
		qDebug() << "[GamePadHandler] handleScreenEvent() - Got device event";

		// A device was attached or removed.
		screen_device_t device;
		int attached;
		int type;

		SCREEN_API(screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void** )&device), "SCREEN_PROPERTY_DEVICE");
		SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ATTACHED, &attached), "SCREEN_PROPERTY_ATTACHED");

		if (attached)
		{
			SCREEN_API(screen_get_device_property_iv(device, SCREEN_PROPERTY_TYPE, &type), "SCREEN_PROPERTY_TYPE");
		}

		qDebug() << "[GamePadHandler] handleScreenEvent() - Got device event for: " << device;

		int i;
		if (attached && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK))
		{
			for (i = 0; i < MAX_CONTROLLERS; ++i)
			{
				if (!mControllers[i].handle)
				{
					mControllers[i].handle = device;
					loadController(&mControllers[i]);
					qDebug() << "[GamePadHandler] handleScreenEvent - Device connected, index: " << i;
					emit gamePadConnected(i);
					break;
				}
			}
		}
		else
		{
			for (i = 0; i < MAX_CONTROLLERS; ++i)
			{
				if (device == mControllers[i].handle)
				{
					initController(&mControllers[i], i);
					qDebug() << "[GamePadHandler] handleScreenEvent - Device disconnected, index: " << i;
					emit gamePadDisconnected(i);
					break;
				}
			}
		}

		break;
	}
	}
}

void GamePadHandler::stop()
{
	qDebug() << "[GamePadHandler] stop() - start";
	shouldRun = false;

	qDebug() << "[GamePadHandler] stop() - end";

}
