#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <SDL2/SDL_ttf.h>
#include "spriteData.h"

class Screen {
public:
    // creates a screen object.
    Screen();

	~Screen();

	// creates a window.
	void makeWindow(int x = 0, int y = 0, int w = 640, int h = 480,
			const char* title = "Snowy", Uint32 pxFormat = SDL_PIXELFORMAT_RGBA8888,
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
	// addToRender true means it will be added directly into the to to-be-rendered vector. Default is false.
	int makeSprite(const char* spriteName, const char* imageName, int layer = 0, SDL_Rect* framesize = nullptr, int tileMarginOffSet = 0, bool isAnimated = true, bool addToRender = false);

    // add a sprite to the to-be-rendered vector and returns 1. Returns 0 if sprite already exists in that vector.
    // Calls Error if sprite doesn't exist.
    int addSpriteToRender(const char* spriteName);

	// retrieve a certain sprite's data so one can modify it. Returns nullpointer if sprite with given name doesn't exist.
	Sprite* spriteData(const char* spriteName);

    // returns a vector containing names with all existing sprites.
    std::vector<std::string> getAllSpriteNames() const;

    // returns a vector containing names with all existing images.
    std::vector<std::string> getAllImageNames() const;

    // returns a vector containing names of all sprites in the to-be-rendered vector.
    std::vector<std::string> getRenderSpriteNames() const;

    // change sprite's layer to a specified layer number.
    void changeLayer(const char* spriteName, int newLayerNumber);

	// duplicates sprite data, if sprite being duplicated doesn't exist or
	// new sprite already exist it doesn't duplicate and returns 0.
	// addToRender true means it will be added directly into the to to-be-rendered vector. Default is false.
	int duplicateSprite(const char* newName, const char* whichSprite, bool addToRender = false);

	// remove a sprites from the to-be-rendered vector, keep the sprite still in the data.
	// returns 1 if successful. Returns 0 if sprite with such name doesn't exist.
    int removeSprite(const char* name);

    // empties the to-be-rendered vector. Returns 1 upon success, 0 if vector was already empty.
    int clearScreen();

	// destroys the sprite: deleted from the to-be-rendered vector and delete it from the data.
	// returns 1 if successful. Returns 0 if sprite with such name doesn't exist.
	int destroySprite(const char* name);

	// removes image, frees up memory it has taken.
	// returns 1 if successful. Returns 0 if image with such name doesn't exist.
	// NOTE: If you remove an image that a sprite still uses will cause undefined behaviour.
	int destroyImg(const char* name);

	// retrieves the screen object
	//static Screen& get();

	// sorting the layers function
	void sortTheRenderingLayers() {
		  std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}

    //-----------------------FONT METHODS-----------------------------------

	// render text into a sprite with given TTF_Font using a render mode:
    //  0=Solid (quick and dirty), this is the default.
    //  1=Blended(Slow, but nicely blended over another image).
    // returns 0 on failure, and 1 on success
    int text(const char* name, std::string message, TTF_Font* fnt, SDL_Color color, int x=0, int y=0, int layer = 1, int mode=0, bool renderImmediately=false);

private:
	SDL_Window *win;
	SDL_Rect cameraView, rectDummy;
	SDL_Renderer *screenRend;
	SDL_Texture *screenTex;

    // functor for sort by layers, returns true if lhs is smaller than rhs.
    class isLessLayer{
    public:
        bool operator() (const Sprite* lhs, const Sprite* rhs) const {
                return (lhs->getLayer() < rhs->getLayer());
        }
    } lessLayer;

	std::unordered_map<const char*, SDL_Texture*> allSprTex;
	std::unordered_map<const char*, Sprite> allSprObj;
	std::vector<Sprite*> spritesToRender;

};

