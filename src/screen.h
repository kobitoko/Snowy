#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include "spriteData.h"

class Screen : protected Sprite {
public:

	~Screen();
	// creates a window.
	void makeWindow(int x = 0, int y = 0, int w = 640, int h = 480,
			const char* title = "Snowy", int framerate = 24,
			Uint32 pxFormat = SDL_PIXELFORMAT_RGBA8888,
			Uint32 renderFlags = SDL_RENDERER_ACCELERATED, Uint32 winFlags = 0);

	// get the window
	SDL_Window* getWindow();

	// get access to the camera's Rect.
	SDL_Rect& getCamera();

	// update the screen
	void update();

	// loads the image and stores it in memory. Returns 0 if an image with the same name already exists.
	int loadImg(const char* imageName, const char* imageLocation);

	// makes a sprite from an existing image in memory. Returns 0 if a sprite with the same name already exists.
	int makeSprite(const char* spriteName, const char* imageName, int layer = 0, SDL_Rect* framesize = nullptr, bool isAnimated = true);

	// retrieve a certain sprite's data so one can modify it.
	Sprite* spriteData(const char* spriteName);

	// duplicates sprite data, if sprite being duplicated doesn't exist or
	// new sprite already exist it doesn't duplicate and returns 0.
	int duplicateSprite(const char* newName, const char* whichSprite);

	// removes the sprite
	void removeSprite(const char* name);

	// removes image, frees up memory it has taken.
	void removeImg(const char* name);

	// returns the FPS
	float getFps() const ;

	// retrieves the screen object
	static Screen& get();

	void sortTheRenderingLayers();

private:
	SDL_Window *win;
	SDL_Rect cameraView, rectDummy;
	int fps;
	SDL_Renderer *screenRend;
	SDL_Texture *screenTex;
	Uint32 msPerFrame;

	Uint32 currTime;
	Uint32 frameSum;
	int frameIndx;
	Uint32* frameList;

	std::map<const char*, SDL_Texture*> allSprTex;
	std::map<const char*, Sprite> allSprObj;
	std::vector<Sprite*> spritesToRender;
	// functor for sort by layers, returns true if lhs is smaller than rhs.
	class isLessLayer{
	public:
		bool operator() (const Sprite* lhs, const Sprite* rhs) const {
				return (lhs->getLayer() < rhs->getLayer());
		}
	} lessLayer;
	// Prevents from making more Screen objects, the object should be retrieved with Screen::get();
	Screen();
};

