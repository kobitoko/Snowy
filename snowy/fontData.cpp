#include "fontData.h"
#include "tools.h"

Fonts::~Fonts() {
    for(auto&& it : allFonts) {
        TTF_CloseFont(it.second);
        it.second = nullptr;
    }
}

int Fonts::createFont(std::string fontName, std::string fontLocation, int pixelHeight, int fontFaceIndex) {
    if(allFonts.count(fontName) > 0)
        return 0;
    allFonts[fontName] = TTF_OpenFontIndex(fontLocation.c_str(), pixelHeight, fontFaceIndex);
    return 1;
}

TTF_Font* Fonts::getFont(std::string fontName) {
    if(allFonts.count(fontName) == 0)
        return nullptr;
    return allFonts[fontName];
}

bool Fonts::fontExists(std::string fontName) const {
    return (allFonts.count(fontName) > 0);
}

int Fonts::destroyFont(std::string fontName) {
	if(allFonts.count(fontName) > 0) {
        TTF_CloseFont(allFonts[fontName]);
		allFonts.erase(fontName);
		return 1;
	}
    return 0;
}

//---------------------Text class------------------------

Text::Text(std::string txtName, std::string theMessage, std::string nameOfFontToUse, int xPos, int yPos,
         SDL_Color colour, int renderingMode, bool shouldRenderIt) :
              name(txtName), message(theMessage), fontName(nameOfFontToUse), x(xPos), y(yPos),
              color(colour), mode(renderingMode), renderIt(shouldRenderIt) {}

std::pair<int, int> Text::getPos() const {
    return std::make_pair(x, y);
}

void Text::setPos(int newX, int newY) {
    x = newX;
    y = newY;
}

SDL_Color Text::getColor() const {
    return color;
}

void Text::setColor(SDL_Color newColor) {
    color = newColor;
}

int Text::getMode() const {
    return mode;
}

void Text::setMode(int newMode) {
    if(newMode == 0 || newMode == 1)
        mode = newMode;
    else
        callError("Text method setMode error for \"" + toStr(name) + "\": Invalid mode," + toStr(mode));
}

bool Text::shouldRender() const {
    return renderIt;
}

void Text::setToRender(bool wannaRenderIt) {
    renderIt = wannaRenderIt;
}
