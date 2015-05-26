#include "screen.h"
#include "fontData.h"

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
    drawTex = nullptr;

	cameraView.x = cameraView.y = cameraView.w = cameraView.h = 0;
	rectDummy.x = rectDummy.y = rectDummy.w = rectDummy.h = 0;
	fontObj = new Fonts();
}

Screen::~Screen() {
    delete fontObj;
    for(auto&& it : allSprTex) {
		SDL_DestroyTexture(it.second);
		it.second = nullptr;
    }
    for(auto&& it : allSprObj) {
        delete it.second;
        it.second = nullptr;
    }
	allSprTex.clear();
	allSprObj.clear();
    SDL_DestroyTexture(screenTex);
    SDL_DestroyTexture(drawTex);
	SDL_DestroyRenderer(screenRend);
	SDL_DestroyWindow(win);
	IMG_Quit();
	TTF_Quit();
}

void Screen::makeWindow(int x, int y, int gameWidth, int gameHeight, int windowWidth, int windowHeight,
                        std::string title, const char* filtering, Uint32 renderFlags, Uint32 winFlags, Uint32 pxFormat) {
	win = SDL_CreateWindow(title.c_str(), x, y, windowWidth, windowHeight, winFlags);
	if(win == nullptr) {
		std::string errmsg = "SDL_CreateWindow error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	screenRend = SDL_CreateRenderer(win, -1, (renderFlags | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED));
	screenTex = SDL_CreateTexture(screenRend, pxFormat, SDL_TEXTUREACCESS_TARGET,gameWidth, gameHeight);
	drawTex = SDL_CreateTexture(screenRend, pxFormat, SDL_TEXTUREACCESS_TARGET, gameWidth, gameHeight);
	//0 or nearest nearest pixel sampling
	//1 or linear linear filtering (supported by OpenGL and Direct3D)
	//2 or best anisotropic filtering (supported by Direct3D)
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, filtering);
    SDL_RenderSetLogicalSize(screenRend, gameWidth, gameHeight);
	cameraView.x = cameraView.y = 0;
	cameraView.w = gameWidth;
	cameraView.h = gameHeight;
}

SDL_Window* Screen::getWindow() {
	return win;
}

SDL_Rect& Screen::getCamera() {
	return cameraView;
}

bool Screen::isInCamera(const SDL_Rect* test) {
    return SDL_IntersectRect(&cameraView, test, &rectDummy);
}

void Screen::renderScreen() {
    SDL_SetRenderDrawColor(screenRend, 0, 0, 0, 255);
    SDL_RenderClear(screenRend);
    // render each sprite onto the screen texture
    //SDL_SetRenderTarget(screenRend, screenTex); // For now just render it directly onto the renderer
    SDL_SetRenderTarget(screenRend, NULL);
    std::pair<float, float> sprOrign{};
    Sprite* spr = nullptr;
    for(size_t i=0; i < spritesToRender.size(); ++i) {
        spr = spritesToRender[i];
        sprOrign = spr->getOrigin();
        const SDL_Point orign = {sprOrign.first, sprOrign.second};
        //if it is within the camera's view render it.
        if(SDL_IntersectRect(&cameraView, spr->getPos(), &rectDummy)) {
            SDL_SetTextureAlphaMod(allSprTex[spr->getImgName()], spr->getAlpha());
            SDL_RenderCopyEx(screenRend, allSprTex[spr->getImgName()],
                             spr->getCurrentFrame(), spr->getPos(),
                             spr->getRotation(), &orign, spr->getFlip());
            //set each sprite's next frame. Handled externally.
            //spr->setNextFrame();
            SDL_SetTextureAlphaMod(allSprTex[spr->getImgName()], 0);
        }
    }
    //SDL_SetRenderDrawColor(screenRend, 0, 255, 0, 255);
    //SDL_RenderDrawLine(screenRend, 0,0,100,100);
}

void Screen::showScreen() {
    // render drawTex textures onto the screenTexture.
    //SDL_RenderCopy(screenRend, drawTex, NULL, NULL);
    // now clear drawTex for next set of drawings.
    //SDL_SetRenderDrawColor(drawRend, 0, 0, 0, 255);
    //SDL_RenderClear(drawRend);

    // render screen texture into the window.
    //SDL_SetRenderTarget(screenRend, NULL);
    //SDL_RenderCopy(screenRend, screenTex, NULL, NULL);
    SDL_RenderPresent(screenRend);
}

void Screen::allRenderSpritesNextFrame() {
    for(size_t i = 0; i < spritesToRender.size(); ++i) {
        spritesToRender[i]->setNextFrame();
    }
}

SDL_Renderer* Screen::getRenderer() {
    return screenRend;
}

SDL_Texture* Screen::getDrawTex() {
	return drawTex;
}

int Screen::loadImg(std::string imageName, std::string imageLocation) {
	if(allSprTex.count(imageName) > 0)
		return 0;
	SDL_Surface* surface = IMG_Load(imageLocation.c_str());
	allSprTex[imageName] = SDL_CreateTextureFromSurface(screenRend, surface);
	SDL_FreeSurface(surface);
	if(allSprTex[imageName] == nullptr)
		callError("screen method loadImg for image \"" + toStr(imageName) + "\" error: " + std::string(IMG_GetError()));
	return 1;
}

int Screen::makeSprite(std::string spriteName, std::string imageName, int layer, SDL_Rect* framesize, int tileMarginOffSet, bool isAnimated, bool addToRender) {
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
		allSprObj[spriteName] = new Sprite(spriteName, imageName, wid, hei, &tmp, layer, isAnimated, tileMarginOffSet);
	} else
		allSprObj[spriteName] = new Sprite(spriteName, imageName, wid, hei, framesize, layer, isAnimated, tileMarginOffSet);
	if(addToRender) {
        spritesToRender.push_back(allSprObj[spriteName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}
	return 1;
}

int Screen::addSpriteToRender(std::string spriteName) {
	if(allSprObj.count(spriteName) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == allSprObj[spriteName])
				return 0;
        spritesToRender.push_back(allSprObj[spriteName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
        return 1;
	}
	callError("screen method addSpriteToRender error: sprite name " + toStr(spriteName) + " does not exist.");
	return 0;
}

Sprite* Screen::getSprite(std::string spriteName) {
	if(allSprObj.count(spriteName) == 0)
			return nullptr;
	return allSprObj[spriteName];
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

bool Screen::spriteExist(std::string name) const{
	return allSprObj.count(name) > 0;
}

bool Screen::textureExist(std::string name) const{
	return allSprTex.count(name) > 0;
}

void Screen::changeLayer(std::string spriteName, int newLayerNumber) {
	getSprite(spriteName)->setLayer(newLayerNumber);
	sortTheRenderingLayers();
}

int Screen::duplicateSprite(std::string newName, std::string whichSprite, bool addToRender) {
	if((allSprObj.count(whichSprite) == 0) || (allSprObj.count(newName) > 0))
		return 0;
    allSprObj[newName] = new Sprite(*allSprObj[whichSprite]);
    allSprObj[newName]->setName(newName);
    if(addToRender) {
        spritesToRender.push_back(allSprObj[newName]);
        std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	}
    return 1;
}

int Screen::removeSprite(std::string name) {
	if(allSprObj.count(name) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == allSprObj[name]) {
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

int Screen::destroySprite(std::string name) {
	if(allSprObj.count(name) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == allSprObj[name]) {
				spritesToRender[i] = spritesToRender.back();
				spritesToRender.pop_back();
				std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
			}
        delete allSprObj[name];
		allSprObj.erase(name);
		return 1;
	}
    return 0;
}

int Screen::destroyImg(std::string name) {
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

int Screen::text(std::string name, std::string message, std::string fontName, SDL_Color color, int x, int y, int layer, int mode, bool renderImmediately) {
    //UTF8
    if(!fontObj->fontExists(fontName))
        callError("screen method text error for \"" + toStr(name) + "\": Font name \"" + toStr(fontName) + "\"does not exist..");
    if(allSprTex.count(name) > 0)
		return 0;
    SDL_Surface* txt = nullptr;
    if(mode == 0)
        txt = TTF_RenderUTF8_Solid(fontObj->getFont(fontName), message.c_str(), color);
    else if(mode == 1)
        txt = TTF_RenderUTF8_Blended(fontObj->getFont(fontName), message.c_str(), color);
    else
        callError("screen method text error for \"" + toStr(name) + "\": Invalid mode," + toStr(mode));
    allSprTex[name] = SDL_CreateTextureFromSurface(screenRend, txt);
    SDL_FreeSurface(txt);
	if(allSprTex[name] == nullptr)
		callError("screen method text error for \"" + toStr(name) + "\": " + std::string(TTF_GetError()));
    if(!makeSprite(name, name, layer, nullptr, 0, false, renderImmediately))
        callError("screen method text error for \"" + toStr(name) + "\": makeSprite failure, sprite name \"" + toStr(name) + "\" already exists.");
    allSprObj[name]->setPos(x,y);
    return 1;
}

Fonts* Screen::getFonts() {
    return fontObj;
}
