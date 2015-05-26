#include "game.h"

Game::~Game() {
    delete[] avgFps;
    delete scr;
    delete in;
    delete snd;
    delete phy;
    delete objMan;
	SDL_Quit();
}
