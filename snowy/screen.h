#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "spriteData.h"
#include "tools.h"

class Fonts;

class Screen {
public:
    /// creates a screen object.
    Screen();

    Screen& operator=(const Screen &rhs){}

	~Screen();

	/// creates a window, gameWidth & height will be resolution of the Renderer. The windowWidth & height
	/// can vary like when maximizing and will then be scaled, with filtering quality as following:
	/// "0" or "nearest" = nearest pixel sampling
	/// "1" or "linear" = linear filtering (supported by OpenGL and Direct3D) (the default)
	/// "2" or "best" = anisotropic filtering (supported by Direct3D)
	/// renderFlags are SDL flags for the renderer, and winFlags are SDL flags for the window. Flags can be OR'd together.
	void makeWindow(int x = 0, int y = 0, int gameWidth = 800, int gameHeight = 600,
                 int windowWidth = 800, int windowHeight = 600, std::string title = "Snowy", const char* filtering = "1",
                 Uint32 renderFlags = 0, Uint32 winFlags = 0, Uint32 pxFormat = SDL_PIXELFORMAT_RGBA8888);

	/// get the window
	SDL_Window* getWindow();

	/// get access to the camera's Rect.
	SDL_Rect& getCamera();

    /// check whether the given SDL_Rect will be in the camera.
    bool isInCamera(const SDL_Rect* test);

	/// draw all the sprite textures to the renderer
	void renderScreen();

	/// shows the screen to the window. Basically SDL_RenderPresent.
	void showScreen();

    /// sets all sprites in the to-be-rendered vector to the next frame.
    void allRenderSpritesNextFrame();

	/// get access to the renderer
    SDL_Renderer* getRenderer();

	/// get access to the drawing texture
    SDL_Texture* getDrawTex();

	/// loads the image and stores it in memory. Returns 0 if an image with the same name already exists.
	int loadImg(std::string imageName, std::string imageLocation);

	/// makes a sprite from an existing image in memory. Returns 0 if a sprite with the same name already exists.
	/// addToRender true means it will be added directly into the to to-be-rendered vector. Default is false.
	int makeSprite(std::string spriteName, std::string imageName, int layer = 0, SDL_Rect* framesize = nullptr, int tileMarginOffSet = 0, bool isAnimated = true, bool addToRender = false);

    /// add a sprite to the to-be-rendered vector and returns 1. Returns 0 if sprite already exists in that vector.
    /// Calls Error if sprite doesn't exist.
    int addSpriteToRender(std::string spriteName);

	/// retrieve a certain sprite's data so one can modify it.
	/// Returns nullpointer if sprite with given name doesn't exist.
	Sprite* getSprite(std::string spriteName);

    /// returns a vector containing names with all existing sprites.
    std::vector<std::string> getAllSpriteNames() const;

    /// returns a vector containing names with all existing images.
    std::vector<std::string> getAllImageNames() const;

    /// returns a vector containing names of all sprites in the to-be-rendered vector.
    std::vector<std::string> getRenderSpriteNames() const;

    /// returns whether the given sprite name exists or not
    bool spriteExist(std::string name) const;

    /// returns whether the given texture name exists or not
    bool textureExist(std::string name) const;

    /// change sprite's layer to a specified layer number.
    void changeLayer(std::string spriteName, int newLayerNumber);

	/// duplicates sprite data, if sprite being duplicated doesn't exist or
	/// new sprite already exist it doesn't duplicate and returns 0.
	/// addToRender true means it will be added directly into the to to-be-rendered vector.
	int duplicateSprite(std::string newName, std::string whichSprite, bool addToRender);

	/// remove a sprites from the to-be-rendered vector, keep the sprite still in the data.
	/// returns 1 if successful. Returns 0 if sprite with such name doesn't exist.
    int removeSprite(std::string name);

    /// empties the to-be-rendered vector. Returns 1 upon success, 0 if vector was already empty.
    int clearScreen();

	/// destroys the sprite: deleted from the to-be-rendered vector and delete it from the data.
	/// returns 1 if successful. Returns 0 if sprite with such name doesn't exist.
	int destroySprite(std::string name);

	/// removes image, frees up memory it has taken.
	/// returns 1 if successful. Returns 0 if image with such name doesn't exist.
	/// NOTE: If you remove an image that a sprite still uses will cause undefined behaviour.
	int destroyImg(std::string name);

	// retrieves the screen object
	//static Screen& get();

	/// sorts all the sprites to be rendered according to the sprite's layers low to high
	/// highest is rendered last & ontop of other layers.
	void sortTheRenderingLayers() {
		  std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}

    //-----------------------FONT METHODS-----------------------------------

	/// render text into a sprite with given TTF_Font using a render mode:
    /// 0=Solid (quick and dirty), this is the default.
    /// 1=Blended(Slow, but nicely blended over another image).
    void text(std::string name, std::string message, std::string fontName, SDL_Color color, int x = 0, int y = 0, int layer = 0, int mode = 0, bool renderImmediately = true);

    /// returns the screen's font container so you can create and store fonts. And recall them easily.
    Fonts* getFonts();

private:
	// don't allow copy construct.
	Screen(const Screen& rhs){}

	SDL_Window *win;
	SDL_Rect cameraView, rectDummy;
	SDL_Renderer *screenRend;
	SDL_Texture *screenTex;
	SDL_Texture *drawTex;

    // functor for sort by layers, returns true if lhs is smaller than rhs.
    class isLessLayer{
    public:
        bool operator() (const Sprite* lhs, const Sprite* rhs) const {
                return (lhs->getLayer() < rhs->getLayer());
        }
    } lessLayer;

	std::unordered_map<std::string, SDL_Texture*> allSprTex;
	std::unordered_map<std::string, Sprite*> allSprObj;
	std::vector<Sprite*> spritesToRender;

    Fonts* fontObj;
};

