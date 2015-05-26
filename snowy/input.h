#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

/// Mouse action names, use mouse::x etc. because to reduce namespace pollution.
enum class MouseVals : int {COORDS, RELMOT, WHEEL};

class Input
{
public:
    /// create an input object
    Input();

    /// creates an event with given desired code, if desired can contain data,
    /// registers it and pushes it into the SDL Events.
    /// Returns 0 when SDL_RegisterEvents fails (not enough user-defined events left).
    int createEvent(Sint32 code, void* anyData = nullptr);

    /// creates an event listener that listens for the created event's code.
    /// Callback function called with anyData as parameter.
    /// returns 0 if code given already exists
    int addEventListener(Sint32 code, void (*callbackFunc)(void*));

    /// removes the eventlistener with the given code. Returns 0 if given code doesnt exist.
    int removeEventListener(Sint32 code);

	/// updates the containers containing some data of inputs.
	void updateInput();

	/// check whether a keyboard key has been pressed. https:///wiki.libsdl.org/SDLKeycodeLookup
	bool keyStatus(SDL_Keycode key);

	/// check whether a window button has been pressed. https:///wiki.libsdl.org/SDL_WindowEventID
	bool windowStatus(Uint8 winID);

	/// check whether a mouse button has been pressed.
	/// Key values are of https://wiki.libsdl.org/SDL_MouseButtonEvent#Remarks
	bool mouseKeyStatus(Uint8 button);

	/// Used to retrieve mouse coordinates, relative motion in the XY direction, or mousewheel XY scroll amount.
	/// Parametes are MouseVals::x where x is one of: COORDS, RELMOT, WHEEL.
	/// NOTE: Calling this multiple times may leave you with different values! Advised to store the returned pair into a variable.
	///   e.g. when you call this once for the X value and then call it again for the Y value.
	/// values are https://wiki.libsdl.org/SDL_MouseMotionEvent and https://wiki.libsdl.org/SDL_MouseWheelEvent
	std::pair<Sint32, Sint32> getMouseValues(MouseVals val);

private:
    // could also use std::function <void(void*)> but needs additional overhead #include <functional>
    typedef void (*callBackFuncName)(void*);
	SDL_Event event;
	std::unordered_map <Sint32, callBackFuncName> evList;
	std::unordered_map <SDL_Keycode, bool> keyActionStatus;
	std::unordered_map <Uint8, bool> windowActionStatus;
	std::unordered_map <Uint8, bool> mouseActionStatus;
	std::unordered_map <int, std::pair<Sint32, Sint32> > mouseValues;

};
