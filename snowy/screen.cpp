#include "screen.h"
#include "errorHandler.h"

Screen::Screen() {
    // Init SDL Video
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::string errmsg = "SDL_Init error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	// Init SDL IMG
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int inits = IMG_Init(flags);
	if((inits & flags) != flags) {
		std::string errmsg = "IMG_Init error: " + std::string(IMG_GetError());
		callError(errmsg);
	}
	// Init SDL TTF
	if(TTF_Init()!= 0) {
        std::string errmsg = "TTF_Init error: " + std::string(TTF_GetError());
        callError(errmsg);
    }
	// perhaps later make a screen into different "cameras" to handle split screen.
	// multiple texture to render on which will be rendered on window then.
	win = nullptr;
	screenRend = nullptr;
	screenTex = nullptr;

	cameraView.x = cameraView.y = cameraView.w = cameraView.h = 0;
	rectDummy.x = rectDummy.y = rectDummy.w = rectDummy.h = 0;
}

Screen::~Screen() {
    for(auto&& it : allSprTex)
		SDL_DestroyTexture(it.second);
	allSprTex.clear();
    SDL_DestroyTexture(screenTex);
	SDL_DestroyRenderer(screenRend);
	SDL_DestroyWindow(win);
	IMG_Quit();
	TTF_Quit();
}

void Screen::makeWindow(int x, int y, int w, int h, const char* title, Uint32 pxFormat, Uint32 renderFlags, Uint32 winFlags) {
	win = SDL_CreateWindow(title, x, y, w, h, winFlags);
	if(win == nullptr) {
		std::string errmsg = "SDL_CreateWindow error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	screenRend = SDL_CreateRenderer(win, -1, (renderFlags | SDL_RENDERER_TARGETTEXTURE));
	screenTex = SDL_CreateTexture(screenRend, pxFormat, SDL_TEXTUREACCESS_TARGET, w, h);
	cameraView.x = cameraView.y = 0;
	cameraView.w = w;
	cameraView.h = h;
}

SDL_Window* Screen::getWindow() {
	return win;
}

SDL_Rect& Screen::getCamera() {
	return cameraView;
}

void Screen::update() {
    SDL_RenderClear(screenRend);
    // render each sprite onto the screen texture
    SDL_SetRenderTarget(screenRend, screenTex);
    for(size_t i=0; i < spritesToRender.size(); ++i) {
        // if it is within the camera's view render it.
        if(SDL_IntersectRect(&cameraView, spritesToRender[i]->getPos(), &rectDummy)) {
            SDL_SetTextureAlphaMod(allSprTex[spritesToRender[i]->getImgName()], spritesToRender[i]->getAlpha());
            SDL_RenderCopyEx(screenRend, allSprTex[spritesToRender[i]->getImgName()],
                    spritesToRender[i]->getCurrentFrame(), spritesToRender[i]->getPos(),
                    spritesToRender[i]->getRotation(), NULL, spritesToRender[i]->getFlip());
            spritesToRender[i]->setNextFrame();
            SDL_SetTextureAlphaMod(allSprTex[spritesToRender[i]->getImgName()], 0);
        }
    }
    // render screen texture into the window.
    SDL_SetRenderTarget(screenRend, NULL);
    // Perhaps later implement so that can change screen's location/rotation etc. and multiple screens in one window etc?
    // Or put everything into OpenGL3.0+ (e.g. SDL in OpenGL mode and render sprites to primitives, applying rotation primitives)?
    SDL_RenderCopy(screenRend, screenTex, NULL, NULL);
    SDL_RenderPresent(screenRend);
}

int Screen::loadImg(const char* imageName, const char* imageLocation) {
	if(allSprTex.count(imageName) > 0)
		return 0;
	SDL_Surface* surface = IMG_Load(imageLocation);
	allSprTex[imageName] = SDL_CreateTextureFromSurface(screenRend, surface);
	SDL_FreeSurface(surface);
	if(allSprTex[imageName] == nullptr)
		callError("screen method loadImg for image \"" + toStr(imageName) + "\" error: " + std::string(IMG_GetError()));
	return 1;
}

int Screen::makeSprite(const char* spriteName, const char* imageName, int layer, SDL_Rect* framesize, int tileMarginOffSet, bool isAnimated, bool addToRender) {
	if(allSprTex.count(imageName) == 0)
		callError("screen method makeSprite error: trying to load non-existing image, " + toStr(imageName));
	if(allSprObj.count(spriteName) > 0)
		return 0;
	int wid = 0;
	int hei = 0;
	SDL_QueryTexture(allSprTex[imageName] , NULL, NULL, &wid, &hei);
	if(framesize == nullptr) {
		SDL_Rect tmp;
		tmp.x = tmp.y = 0;
		tmp.w = wid;
		tmp.h = hei;
		allSprObj[spriteName] = Sprite(spriteName, imageName, wid, hei, &tmp, layer, isAnimated, tileMarginOffSet);
	} else
		allSprObj[spriteName] = Sprite(spriteName, imageName, wid, hei, framesize, layer, isAnimated, tileMarginOffSet);
	if(addToRender) {
        spritesToRender.push_back(&allSprObj[spriteName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}
	return 1;
}

int Screen::addSpriteToRender(const char* spriteName) {
	if(allSprObj.count(spriteName) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == &allSprObj[spriteName])
				return 0;
        spritesToRender.push_back(&allSprObj[spriteName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
        return 1;
	}
	callError("screen method addSpriteToRender error: sprite name " + toStr(spriteName) + " does not exist.");
	return 0;
}

Sprite* Screen::spriteData(const char* spriteName) {
	if(allSprObj.count(spriteName) == 0)
			return nullptr;
	return &allSprObj[spriteName];
}

std::vector<std::string> Screen::getAllSpriteNames() const{
    std::vector<std::string> ret;
    for(auto& a : allSprObj)
        ret.push_back(a.first);
    return ret;
}

std::vector<std::string> Screen::getAllImageNames() const{
    std::vector<std::string> ret;
    for(auto& a : allSprTex)
        ret.push_back(a.first);
    return ret;
}

std::vector<std::string> Screen::getRenderSpriteNames() const{
    std::vector<std::string> ret;
    for(size_t i=0; i < spritesToRender.size(); ++i)
        ret.push_back(spritesToRender[i]->getName());
    return ret;
}

void Screen::changeLayer(const char* spriteName, int newLayerNumber) {
	spriteData(spriteName)->setLayer(newLayerNumber);
	sortTheRenderingLayers();
}

int Screen::duplicateSprite(const char* newName, const char* whichSprite, bool addToRender) {
	if((allSprObj.count(whichSprite) == 0) || (allSprObj.count(newName) > 0))
		return 0;
	allSprObj[newName] = allSprObj[whichSprite];
	allSprObj[newName].setName(newName);
	if(addToRender) {
        spritesToRender.push_back(&allSprObj[newName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}
	return 1;
}

int Screen::removeSprite(const char* name) {
	if(allSprObj.count(name) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == &allSprObj[name]) {
				spritesToRender[i] = spritesToRender.back();
				spritesToRender.pop_back();
				std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
			}
        return 1;
	}
	return 0;
}

int Screen::clearScreen() {
    if(!spritesToRender.empty()) {
        spritesToRender.clear();
        return 1;
    }
    return 0;
}

int Screen::destroySprite(const char* name) {
	if(allSprObj.count(name) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == &allSprObj[name]) {
				spritesToRender[i] = spritesToRender.back();
				spritesToRender.pop_back();
				std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
			}
		allSprObj.erase(name);
		return 1;
	}
    return 0;
}

int Screen::destroyImg(const char* name) {
	if(allSprTex.count(name) > 0) {
		SDL_DestroyTexture(allSprTex[name]);
		allSprTex.erase(name);
		return 1;
	}
	return 0;
}

/*
Screen& Screen::get() {
	static Screen screenObj;
	return screenObj;
}
*/

//-----------------------FONT METHODS-----------------------------------

int Screen::text(const char* name, std::string message, TTF_Font* fnt, SDL_Color color, int x, int y, int layer, int mode, bool renderImmediately) {
    //UTF8
    if(fnt == nullptr)
        callError("screen method text error for \"" + toStr(name) + "\": TTF_Font is nullptr.");
    if(allSprTex.count(name) > 0)
		return 0;
    SDL_Surface* txt;
    if(mode == 0)
        txt = TTF_RenderUTF8_Solid(fnt, message.c_str(), color);
    else if(mode == 1)
        txt = TTF_RenderUTF8_Blended(fnt, message.c_str(), color);
    else
        callError("screen method text error for \"" + toStr(name) + "\": Invalid mode," + toStr(mode));
    allSprTex[name] = SDL_CreateTextureFromSurface(screenRend, txt);
    SDL_FreeSurface(txt);
	if(allSprTex[name] == nullptr)
		callError("screen method text error for \"" + toStr(name) + "\": " + std::string(TTF_GetError()));
    if(!makeSprite(name, name, layer, nullptr, 0, false, renderImmediately))
        callError("screen method text error for \"" + toStr(name) + "\": makeSprite failure, sprite name \"" + toStr(name) + "\" already exists.");
    allSprObj[name].setPos(x,y);
    return 1;
}
