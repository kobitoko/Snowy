#include "spriteData.h"
#include "errorHandler.h"

// empty sprite shell
Sprite::Sprite() :
		name(0), img(0), totalW(0), totalH(0), onLayer(0), isAnim(false), maxFramesHor(0),
		maxFramesVer(0), maxFrames(0), currFrame(0), currCustomFrame(0),
		rotAng(0), flip(SDL_FLIP_NONE), frameLonger(0), frameSuspended(0), alphaVal(255) {
	frameSize.x = frameSize.y = frameSize.w = frameSize.h = 0;
	currFrameRect.x = currFrameRect.y = currFrameRect.w = currFrameRect.h = 0;
	posStretch.x = posStretch.y = posStretch.w = posStretch.h = 0;
}

// an actual sprite is being made
Sprite::Sprite(const char* spriteName, const char* imgName, int imgWidth, int imgHeight,
		SDL_Rect* framesize, int layer, bool isAnimated)  :
			totalW(imgWidth), totalH(imgHeight), onLayer(layer),
			isAnim(isAnimated), currFrame(0), currCustomFrame(0), rotAng(0), flip(SDL_FLIP_NONE),
			frameLonger(0),	frameSuspended(0), alphaVal(255) {
	frameSize.x = framesize->x;
	frameSize.y = framesize->y;
	frameSize.w = framesize->w;
	frameSize.h = framesize->h;
	name = spriteName;
	img = imgName;
	if(frameSize.w > totalW || frameSize.h > totalH) {
        std::string a = "Sprite constructor error for sprite " + toStr(name) + ": FrameSize cannot be larger than the actual image size. \n";
        std::string b = "FrameSize is " + toStr(frameSize.w) + "x" + toStr(frameSize.h) + "\n";
        std::string c = "Image size is " + toStr(totalW) + "x" + toStr(totalH);
		callError( a + b + c);
	}
	maxFramesHor = totalW / frameSize.w;
	maxFramesVer = totalH / frameSize.h;
	maxFrames = maxFramesHor * maxFramesVer;
	if(maxFrames == 0) {
		maxFramesHor = 1;
		maxFramesVer = 1;
		maxFrames = 1;
	}
	currFrameRect.x = frameSize.x;
	currFrameRect.y = frameSize.y;
	currFrameRect.w = frameSize.w;
	currFrameRect.h = frameSize.h;

	// position on screen and stretch of image
	posStretch.x = 0;
	posStretch.y = 0;
	posStretch.w = frameSize.w;
	posStretch.h = frameSize.h;

  frameOrder.clear();
}

// Copy Constructor
Sprite::Sprite(const Sprite& rhs) :
	totalW(rhs.totalW), totalH(rhs.totalH), frameSize(rhs.frameSize),
	onLayer(rhs.onLayer),isAnim(rhs.isAnim), currFrame(rhs.currFrame),
	currCustomFrame(rhs.currCustomFrame), currFrameRect(rhs.currFrameRect),
	maxFramesHor(rhs.maxFramesHor), maxFramesVer(rhs.maxFramesVer),
		maxFrames(rhs.maxFrames), posStretch(rhs.posStretch), frameOrder(rhs.frameOrder),
	rotAng(rhs.rotAng), flip(rhs.flip),	frameLonger(rhs.frameLonger), alphaVal(rhs.alphaVal) {
	name = rhs.name;
	img = rhs.img;
	frameSuspended = 0;
}

// Assignment Operator
Sprite& Sprite::operator=(const Sprite &rhs) {
	// protect from self assignment
	if(this != &rhs) {
		name = rhs.name;
		img = rhs.img;
		onLayer = rhs.onLayer;
		totalW = rhs.totalW;
		totalH = rhs.totalH;
		frameSize = rhs.frameSize;
		currFrameRect = rhs.currFrameRect;
		maxFramesHor = rhs.maxFramesHor;
		maxFramesVer = rhs.maxFramesVer;
		maxFrames = rhs.maxFrames;
		currFrame = rhs.currFrame;
		posStretch = rhs.posStretch;
		isAnim = rhs.isAnim;
		currCustomFrame = rhs.currCustomFrame;
		rotAng = rhs.rotAng;
		flip = rhs.flip;
		frameOrder = rhs.frameOrder;
		frameLonger = rhs.frameLonger;
		frameSuspended = 0;
		alphaVal = rhs.alphaVal;
	}
	return *this;
}

int Sprite::getImageSizeW() const {
	return totalW;
}

int Sprite::getImageSizeH() const {
	return totalH;
}

int Sprite::getFrameSizeW() const {
	return frameSize.w;
}

int Sprite::getFrameSizeH() const {
	return frameSize.h;
}

int Sprite::currentFrameNum() const {
	return currFrame;
}

int Sprite::maxFrame() const {
	return maxFrames;
}

const SDL_Rect* Sprite::getCurrentFrame() const {
	return &currFrameRect;
}

void Sprite::setNextFrame() {
	if(maxFrames > 1 && isAnim) {
		// only draw if frame is more than frame delay.
		if(frameSuspended >= frameLonger) {
			// set next frame number
			if(!frameOrder.empty()) {
        ++currCustomFrame;
				if(static_cast<size_t>(currCustomFrame) >= frameOrder.size())
					currCustomFrame = 0;
				currFrame = frameOrder[currCustomFrame];
			} else {
        ++currFrame;
			}
			// actually change the frame
			setCurrFrameRect();
			frameSuspended = 0;
		}
		++frameSuspended;
	}
}

void Sprite::setFrame(int frameNum) {
	if(frameNum >= (maxFrames) || frameNum < 0)
		callError("Sprite method setFrame error for sprite " + toStr(name) + ": parameter has an invalid value, value is " + toStr(frameNum) + " maximum frame number is" + toStr(frameNum));
	// if custom frame order is defined, then change gets overwritten by custom frame
	currFrame = frameNum;
	// if it is not animated sprite, change the frameRect now since setNextFrame will not do it.
	if(!isAnim)
		setCurrFrameRect();
}

void Sprite::customFrameOrder(std::vector<int>& customFrameOrder) {
	if (customFrameOrder.size() == 0)
		callError("Sprite method customFrameOrder error for sprite " + toStr(name) + ": passing an empty vector");
	for(size_t i=0; i<customFrameOrder.size(); ++i) {
		if(customFrameOrder[i] >= maxFrames || customFrameOrder[i] < 0)
			callError("Sprite method customFrameOrder error for sprite " + toStr(name) + ": a value exceeds maxFrames");
	}
	frameOrder = customFrameOrder;
	currCustomFrame = 0;
	currFrame = frameOrder[currCustomFrame];
	setCurrFrameRect();
}

void Sprite::disableCustomFrameOrder() {
	frameOrder.clear();
}

const char* Sprite::getName() const {
	return name;
}

const char* Sprite::setName(const char* newName) {
    name = newName;
    return name;
}

const char* Sprite::getImgName() const {
	return img;
}


int Sprite::getLayer() const {
	return onLayer;
}

void Sprite::setLayer(int layerNum) {
	onLayer = layerNum;
}

const SDL_Rect* Sprite::getPos() const {
	return &posStretch;
}

void Sprite::setPos(int x, int y) {
	posStretch.x = x;
	posStretch.y = y;
}

void Sprite::setStretch(int ws, int hs) {
	if(ws <= 0 || hs <= 0)
		callError("Sprite method setStretch error for sprite " + toStr(name) + ": invalid value given. Values have to be positive.");

	posStretch.w = ws;
	posStretch.h = hs;
}

void Sprite::resetStretch() {
	posStretch.w = frameSize.w;
	posStretch.h = frameSize.h;
}

void Sprite::rotation(double rotat) {
	rotAng = rotat;
}
double Sprite::getRotation() const{
	return rotAng;
}
void Sprite::flipHor() {
	flip = SDL_FLIP_HORIZONTAL;
}
void Sprite::flipVert() {
	flip = SDL_FLIP_VERTICAL;
}
void Sprite::flipDiag() {
	flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
}
void Sprite::flipNone() {
	flip = SDL_FLIP_NONE;
}
SDL_RendererFlip Sprite::getFlip() const {
	return flip;
}
void Sprite::delayFrames(int amount) {
	if(amount < 0 )
		callError("Sprite method delayFrames error for sprite " + toStr(name) + ": invalid value given. Value has to be positive");
	frameLonger = amount;
}
int Sprite::getDelayedFrames() const {
	return frameLonger;
}

Uint8 Sprite::getAlpha() const {
	return alphaVal;
}

void Sprite::setAlpha(Uint8 value) {
	alphaVal = value;
}

// ----------------------------- Private functions -------------------------------------

void Sprite::setCurrFrameRect() {
	if(currFrame >= maxFrames) {
		currFrame = 0;
		currFrameRect.y = 0;
	}
	else if(currFrame >= maxFramesHor)
		currFrameRect.y = (currFrame/maxFramesHor) * frameSize.h;
	else if(currFrame < maxFramesHor)
		currFrameRect.y = 0;
	currFrameRect.x = ((currFrame % maxFramesHor) * frameSize.w);
}
