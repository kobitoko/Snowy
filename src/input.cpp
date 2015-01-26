#include "input.h"
#include "errorHandler.h"

Input::Input() {
	if( SDL_Init(SDL_INIT_EVENTS) !=0) {
		std::string errmsg = "SDL_Init error:" + std::string(SDL_GetError());
		callError(errmsg);
	}
}

Input& Input::get() {
	static Input inputMaster;
	return inputMaster;
}

void Input::updateInput() {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		// To do: Handle more event types https://wiki.libsdl.org/SDL_EventType
		//  and https://wiki.libsdl.org/CategoryGameController

		// Keyboard events
		case (SDL_KEYDOWN): {
			keyActionStatus[event.key.keysym.sym] = true;
			break;
		}
		case (SDL_KEYUP): {
			keyActionStatus[event.key.keysym.sym] = false;
			break;
		}

		// Window Events
		case(SDL_WINDOWEVENT): {
			windowActionStatus[event.window.event] = true;
			break;
		}

		// Mouse motion
		case(SDL_MOUSEMOTION): {
			mouseValues[int(MouseVals::COORDS)] = std::pair<Sint32, Sint32>(event.motion.x, event.motion.y);
			mouseValues[int(MouseVals::RELMOT)] = std::pair<Sint32, Sint32>(event.motion.xrel, event.motion.yrel);
			break;
		}

		// Mouse wheel
		case(SDL_MOUSEWHEEL): {
			mouseValues[int(MouseVals::WHEEL)] = std::pair<Sint32, Sint32>(event.wheel.x, event.wheel.y);
			break;
		}

		// Mouse Buttons
		// SDL_TOUCH_MOUSEID not filtered out, thus touch input devices included.
		case(SDL_MOUSEBUTTONDOWN): {
				mouseActionStatus[event.button.button] = true;
				break;
		}
		case(SDL_MOUSEBUTTONUP): {
				mouseActionStatus[event.button.button] = false;
				break;
		}

		} //end switch
	}
}

bool Input::keyStatus(SDL_Keycode key) {
	return keyActionStatus[key];
}

bool Input::windowStatus(Uint8 winID) {
	bool status = windowActionStatus[winID];
	if(status) {
		// assume the event is handled and reset the window event
		windowActionStatus[winID] = false;
		return true;
	}
	return status;
}

bool Input::mouseKeyStatus(Uint8 button) {
	return mouseActionStatus[button];
}

std::pair<Sint32, Sint32> Input::getMouseValues(MouseVals val) {
	if(val == MouseVals::WHEEL || val == MouseVals::RELMOT) {
		std::pair<Sint32, Sint32> theVals = mouseValues[int(val)];
		mouseValues[int(val)] = std::pair<Sint32, Sint32>(0, 0);
		return theVals;
	}
	return mouseValues[int(val)];
}
