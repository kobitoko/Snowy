#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Sprite {
public:
	Sprite();

	Sprite(std::string spriteName, std::string imgName, int imgWidth, int imgHeight, SDL_Rect* framesize, int layer = 0, bool isAnimated = true, int spaceBetweenTiles = 0);
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite &rhs);
	~Sprite() { }

	/// returns the width and height size of the complete image
	std::pair<int, int> getImageSize() const;

	/// return the width and height of the individual frames.
    std::pair<int, int> getFrameSize() const;

    /// return the margin, the empty space between each frame.
	int getMargin() const;

	///get the origin values x and y of the sprite.
	std::pair<float,float> getOrigin() const;

    /// set the origin value of the sprite.
    void setOrigin(float x, float y);

    /// whether the sprite has an animation.
    bool isAnimation() const;

	/// retrieve the current frame number that the sprite is on.
	int currentFrameNum() const;

	/// retrieve the maximum amount of frames the sprite has.
	int maxFrame() const;

	/// returns the SDL_Rect info for the currentframe of the sprite to draw.
	const SDL_Rect* getCurrentFrame() const;

	/// tells the sprite to move on to the next frame
	void setNextFrame();

	/// set frame to a specific frame, counting starts at 0, so n-1 where n is the total # of frames.
	/// Note that it gets overwritten by custom frame when a custom frame order is defined and setNextFrame() is called.
	void setFrame(int frameNum);

	/// retrieve the sprite's name.
	std::string getName() const;

	/// change the sprite's name, returns the new name.
    std::string setName(std::string newName);

	/// retrieve the location string of the image that this sprite uses.
	std::string getImgName() const;

    /// returns the layer number the sprite prefers to be on.
    int getLayer() const;

    /// sets the layer number the sprite prefers to be on.
    void setLayer(int layerNum);

	/// get the position and stretch of the sprite.
	const SDL_Rect* getPos() const;

    /// get the top-left position of the sprite through an int pair.
    std::pair<int, int> getPosInts() const;

	/// set top-left position on the screen for the sprite.
	void setPos(int x, int y);

	/// stretch the image to fit a given size in pixels. Cannot be 0 or a negative value!
	/// automatically resets the origin of the sprite to center of the new stretch size.
	void setStretch(int ws, int hs);

	/// reset stretch to original width and height.
	void resetStretch();

	/// rotate the sprite by a given angle degeree
	void rotation(double rotat);

	/// retrieve the sprite's rotation.
	double getRotation() const;

	/// flip the image horizontally
	void flipHor();

	/// flip the image vertically
	void flipVert();

	/// flip the image diagonally
	void flipDiag();

	/// reset the flip of the image
	void flipNone();

	/// retrive the sprite's current flip of its image
	SDL_RendererFlip getFlip() const;

	/// set a custom frame order to iterate through. Frame number cannot be < 0 and >= max-frame-number.
	void customFrameOrder(std::vector<int>& customFrameOrder);

	/// clear the custom frame order (simply empties the vector container).
	void disableCustomFrameOrder();

	/// the amount of additional frames to wait before going to the next frame. 0 = original speed.
	void delayFrames(int amount);

	/// retrieve the amount of delayed frames for this sprite.
	int getDelayedFrames() const;

	/// gets sprites alpha value *NOTE: Useless if the image of this sprite is not a PNG or file without alpha support.
	Uint8 getAlpha() const;

	/// sets sprites alpha value *NOTE: Useless if the image of this sprite is not a PNG or file without alpha support.
	void setAlpha(Uint8 value);

private:
	// variables
	std::string name;
	std::string img;
	int totalW;
	int totalH;
	SDL_Rect frameSize;
	int onLayer;
	bool isAnim;
	SDL_Rect currFrameRect;
	int maxFramesHor;
	int maxFramesVer;
	int maxFrames;
	int offsetpx;

	int currFrame;
	int currCustomFrame;
	SDL_Rect posStretch;
	std::vector<int> frameOrder;

	double rotAng;
	SDL_RendererFlip flip;
	int frameLonger;
	int frameSuspended;
	Uint8 alphaVal;

    float centerPointX;
    float centerPointY;

	// private function
	void setCurrFrameRect();

};
