#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

/// Font class can load and keep track of all ttf fonts, upon destruction it will free all TTF_Font*
class Fonts {
public:
    Fonts() {}
    ~Fonts();

    /// creates TTF_Font* and puts it in a container. Returns 0 if such a fontname already exists.
    int createFont(std::string fontName,std::string fontLocation, int pixelHeight = 18, int fontFaceIndex = 0);

    /// returns nullptr if there is no font with such name in the container.
    TTF_Font* getFont(std::string fontName);

    /// returns whether the font exists or not.
    bool fontExists(std::string fontName) const;

    /// returns 1 if successfully destroyed font, returns 0 if no such fontName exist.
    int destroyFont(std::string fontName);

private:
    std::unordered_map<std::string, TTF_Font*> allFonts;
};


class Text {
public:
    Text() : name(""), message(""), fontName(""), x(0), y(0), color({0,0,0}), mode(0), renderIt(false) {}
    Text(std::string txtName, std::string theMessage, std::string nameOfFontToUse, int xPos = 0, int yPos = 0,
         SDL_Color colour = {0,0,0}, int renderingMode = 0, bool shouldRenderIt = true);
    ~Text() {}

    std::pair<int, int> getPos() const;
    void setPos(int newX, int newY);
    SDL_Color getColor() const;
    void setColor(SDL_Color newColor);
    int getMode() const;
    void setMode(int newMode);
    bool shouldRender() const;
    void setToRender(bool wannaRenderIt);

private:
    std::string name;
    std::string message;
    std::string fontName;
    int x;
    int y;
    SDL_Color color;
    int mode;
    bool renderIt;
};
