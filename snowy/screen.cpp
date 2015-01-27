#include "screen.h"
#include "errorHandler.h"

Screen::Screen() {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::string errmsg = "SDL_Init error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int inits = IMG_Init(flags);
	if((inits & flags) != flags) {
		std::string errmsg = "IMG_Init error: " + std::string(IMG_GetError());
		callError(errmsg);
	}
	// perhaps later make a screen into "sprites" to handle split screen? idk.
	//  ^ works fine with multiple texture to render on. which will be rendered on window then.
	win = nullptr;
	fps = 0;
	screenRend = nullptr;
	screenTex = nullptr;
	msPerFrame = 0;

	frameSum = 0;
	frameIndx = 0;
	frameList = new Uint32[50]();
	currTime = 0;

	cameraView.x = cameraView.y = cameraView.w = cameraView.h = 0;
	rectDummy.x = rectDummy.y = rectDummy.w = rectDummy.h = 0;
}

Screen::~Screen() {
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(screenRend);
	SDL_DestroyTexture(screenTex);
	for(auto&& it : allSprTex) {
		SDL_DestroyTexture(it.second);
		allSprTex.erase(it.first);
	}
	IMG_Quit();
}

void Screen::makeWindow(int x, int y, int w, int h, const char* title, int framerate, Uint32 pxFormat, Uint32 renderFlags, Uint32 winFlags) {
	win = SDL_CreateWindow(title, x, y, w, h, winFlags);
	if(win == nullptr) {
		std::string errmsg = "SDL_CreateWindow error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	fps = framerate;
	msPerFrame = 1000 / fps;
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
	static Uint32 timeToPass = 0;
	if(SDL_TICKS_PASSED(SDL_GetTicks(), timeToPass)) {
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

		// time it took to get this frame rendered, adopted from http://stackoverflow.com/a/87732
		frameSum -= frameList[frameIndx];
		currTime = SDL_GetTicks() - (timeToPass - msPerFrame);
		frameSum += currTime;
		frameList[frameIndx] = currTime;
		if(++frameIndx == 50)
			frameIndx = 0;
		// The new time that the next frame is due.
		timeToPass = SDL_GetTicks() + msPerFrame;
	}
}

int Screen::loadImg(const char* imageName, const char* imageLocation) {
	if(allSprTex.count(imageName) > 0)
		return 0;
	SDL_Surface* surface = IMG_Load(imageLocation);
	allSprTex[imageName] = SDL_CreateTextureFromSurface(screenRend, surface);
	SDL_FreeSurface(surface);
	if(allSprTex[imageName] == nullptr) {
		std::string errmsg = "render method makeSprite: SDL_CreateTextureFromSurface error: " + std::string(IMG_GetError());
		callError(errmsg);
	}
	return 1;
}

int Screen::makeSprite(const char* spriteName, const char* imageName, int layer, SDL_Rect* framesize, bool isAnimated) {
	if(allSprTex.count(imageName) == 0)
		callError("render method makeSprite error: trying to load non-existing image.");
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
		allSprObj[spriteName] = Sprite(spriteName, imageName, wid, hei, &tmp, layer, isAnimated);
	} else
		allSprObj[spriteName] = Sprite(spriteName, imageName, wid, hei, framesize, layer, isAnimated);
	spritesToRender.push_back(&allSprObj[spriteName]);
	std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	return 1;
}

Sprite* Screen::spriteData(const char* spriteName) {
	if(allSprObj.count(spriteName) == 0)
			return nullptr;
	return &allSprObj[spriteName];
}

int Screen::duplicateSprite(const char* newName, const char* whichSprite) {
	if((allSprObj.count(whichSprite) == 0) || (allSprObj.count(newName) > 0))
		return 0;
	allSprObj[newName] = allSprObj[whichSprite];
	spritesToRender.push_back(&allSprObj[newName]);
	std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
	return 1;
}

void Screen::removeSprite(const char* name) {
	if(allSprObj.count(name) > 0) {
		for(size_t i=0; i < spritesToRender.size(); ++i)
			if(spritesToRender[i] == &allSprObj[name]) {
				spritesToRender[i] = spritesToRender.back();
				spritesToRender.pop_back();
				std::sort(spritesToRender.begin(), spritesToRender.end(), lessLayer);
			}
		allSprObj.erase(name);
	}
}

void Screen::removeImg(const char* name) {
	if(allSprTex.count(name) > 0) {
		SDL_DestroyTexture(allSprTex[name]);
		allSprTex.erase(name);
	}
}

float Screen::getFps() const {
	float averageFrame = static_cast<float>(frameSum) / 50.0f;
	return (1000.0f / averageFrame);
}

Screen& Screen::get() {
	static Screen screenObj;
	return screenObj;
}

