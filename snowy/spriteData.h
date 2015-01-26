#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Sprite {
public:
	Sprite();

	Sprite(const char* spriteName, const char* imgName, int imgWidth, int imgHeight, SDL_Rect* framesize, int layer = 0, bool isAnimated = true);
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite &rhs);
	~Sprite() { }

	// returns the width size of the complete image
	int getImageSizeW() const;

	// returns the height size of the complete image
	int getImageSizeH() const;

	// return the width of the individual frames.
	int getFrameSizeW() const;

	// return the height of the individual frames.
	int getFrameSizeH() const;

	// retrieve the current frame number that the sprite is on.
	int currentFrameNum() const;

	// retrieve the maximum amount of frames the sprite has.
	int maxFrame() const;

	// returns the SDL_Rect info for the currentframe of the sprite to draw.
	const SDL_Rect* getCurrentFrame() const;

	// tells the sprite to move on to the next frame
	void setNextFrame();

	// set frame to a specific frame, counting starts at 0, so n-1 where n is the total # of frames.
	void setFrame(int frameNum);

	// the layer that is bigger will be drawn first.
	void changeLayer(int layer);

	// retrieve the layer that the sprite is on.
	int getLayer() const;

	// retrieve the sprite's name.
	const char* getName() const;

	// retrieve the location string of the image that this sprite uses.
	const char* getImgName() const;

	// get the position and stretch of the sprite.
	const SDL_Rect* getPos() const;

	// set position on the screen.
	void setPos(int x, int y);

	// stretch the image to fit a given size in pixels. Cannot be 0 or a negative value!
	void setStretch(int ws, int hs);

	// reset stretch to original width and height.
	void resetStretch();

	// rotate the sprite by a given angle degeree
	void rotation(double rotat);

	// retrieve the sprite's rotation.
	double getRotation() const;

	// flip the image horizontally
	void flipHor();

	// flip the image vertically
	void flipVert();

	// flip the image diagonally
	void flipDiag();

	// reset the flip of the image
	void flipNone();

	// retrive the sprite's current flip of its image
	SDL_RendererFlip getFlip() const;

	// set a custom frame order to iterate through. Frame number cannot be < 0 and >= max-frame-number.
	void customFrameOrder(std::vector<int>& customFrameOrder);

	// clear the custom frame order (simply empties the vector container).
	void disableCustomFrameOrder();

	// the amount of additional frames to wait before going to the next frame. 0 = original speed.
	void delayFrames(int amount);

	// retrieve the amount of delayed frames for this sprite.
	int getDelayedFrames() const;

	// gets sprites alpha value *NOTE: Useless if the image of this sprite is not a PNG or file without alpha support.
	Uint8 getAlpha() const;

	// sets sprites alpha value *NOTE: Useless if the image of this sprite is not a PNG or file without alpha support.
	void setAlpha(Uint8 value);

private:
	// variables
	const char* name;
	const char* img;
	int totalW;
	int totalH;
	SDL_Rect frameSize;
	int onLayer;
	bool isAnim;
	SDL_Rect currFrameRect;
	int maxFramesHor;
	int maxFramesVer;
	int maxFrames;
	int currFrame;
	SDL_Rect posStretch;
	std::vector<int> frameOrder;
	int currCustomFrame;
	double rotAng;
	SDL_RendererFlip flip;
	int frameLonger;
	int frameSuspended;
	Uint8 alphaVal;
	// private function
	void setCurrFrameRect();
};
