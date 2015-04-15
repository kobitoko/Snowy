#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

// Mouse action names, use mouse::x etc. because to reduce namespace pollution.
enum class MouseVals : int {COORDS, RELMOT, WHEEL};

class Input
{
public:
    // create an input object
    Input();

	// updates the containers containing some data of inputs.
	void updateInput();

	// check whether a keyboard key has been pressed. https://wiki.libsdl.org/SDLKeycodeLookup
	bool keyStatus(SDL_Keycode key);

	// check whether a window button has been pressed. https://wiki.libsdl.org/SDL_WindowEventID
	bool windowStatus(Uint8 winID);

	// check whether a mouse button has been pressed.
	// Key values are of https://wiki.libsdl.org/SDL_MouseButtonEvent#Remarks
	bool mouseKeyStatus(Uint8 button);

	// Used to retrieve mouse coordinates, relative motion in the XY direction, or mousewheel XY scroll amount.
	//   https://wiki.libsdl.org/SDL_MouseMotionEvent https://wiki.libsdl.org/SDL_MouseWheelEvent
	// Parametes are MouseVals::x where x is one of: COORDS, RELMOT, WHEEL.
	// NOTE: Calling this multiple times may leave you with different values! Advised to store the returned pair into a variable.
	//   e.g. when you call this once for the X value and then call it again for the Y value.
	std::pair<Sint32, Sint32> getMouseValues(MouseVals val);

private:
	SDL_Event event;
	std::unordered_map <SDL_Keycode, bool> keyActionStatus;
	std::unordered_map <Uint8, bool> windowActionStatus;
	std::unordered_map <Uint8, bool> mouseActionStatus;
	std::unordered_map <int, std::pair<Sint32, Sint32> > mouseValues;

};
