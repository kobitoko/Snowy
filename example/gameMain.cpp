#include "game.h"
#include <string>
#include <sstream>
#include <cmath>

// SDL2 needs main() to be main(int argc, char * argv[]) and not the simple main()
// https://wiki.libsdl.org/FAQWindows#I_get_.22Undefined_reference_to_.27SDL_main->27.22_...
int main(int argc, char * argv[]) {

    // core game logic updates will 60Hz
    float fixedTime = 1.0f / 60.0f;
    // fps for all sprites is at 30fps
    float sprFps = 1.0f / 30.0f;
    // fps capped at 900fps, basically as fast as possible.
    float cappedFps = 1.0f / 900.0f;
    // preventing spiral of death.
    int maxSteps = 8;

    const bool VSYNC = false;
    const char* filtering = "linear";
	int x = SDL_WINDOWPOS_CENTERED;
	int y = SDL_WINDOWPOS_CENTERED;

	Game *game = new Game(fixedTime, sprFps, cappedFps, maxSteps);

	game->w = 800;
	game->h = 600;

    game->scr = new Screen();
    game->in = new Input();
    game->snd = new Sound();
    game->phy = new Physics(game->GRAVITY, 0.0f, 10.0f, fixedTime);
    game->objMan = new ObjectManager();
    game->cam = new Camera();
    game->fpsTimer = Timer();

    /* SDL2 window flags:
    SDL_WINDOW_FULLSCREEN fullscreen window
    SDL_WINDOW_FULLSCREEN_DESKTOP fullscreen window at the current desktop resolution
    SDL_WINDOW_OPENGL window usable with OpenGL context
    SDL_WINDOW_HIDDEN window is not visible
    SDL_WINDOW_BORDERLESS no window decoration
    SDL_WINDOW_RESIZABLE window can be resized
    SDL_WINDOW_MINIMIZED window is minimized
    SDL_WINDOW_MAXIMIZED window is maximized
    SDL_WINDOW_INPUT_GRABBED window has grabbed input focus
    SDL_WINDOW_ALLOW_HIGHDPI window should be created in high-DPI mode if supported
    */
    if(VSYNC)
        game->scr->makeWindow(x, y, game->w, game->h, 800, 600, "Surf!", filtering,
                                SDL_RENDERER_PRESENTVSYNC, SDL_WINDOW_RESIZABLE, SDL_PIXELFORMAT_RGBA8888);
    else
        game->scr->makeWindow(x, y, game->w, game->h, 800, 600, "Surf!", filtering,
                                0, SDL_WINDOW_RESIZABLE, SDL_PIXELFORMAT_RGBA8888);

	//SDL_SetWindowBordered(window.getWindow(),SDL_FALSE);
	std::string winName = "Surf!";

	// load the test sprites
	game->loadMySprites();

	// load sounds n music
	game->loadMySounds();

	// load the physics
	game->loadMyPhysics();
	game->loadMyParticles();
	game->loadMyJelly();

    //debugdraw at global scope
    game->dbgdrw = new DebugDraw(game->MTOPX, game->scr, game->cam);

    //set debugdraw in box2D.
    game->phy->getWorld()->SetDebugDraw(game->dbgdrw);
    //debugdraw somewhere appropriate b2Draw:: e_shapeBit ( draw shapes ), e_particleBit ( draw particles ),
    // e_jointBit ( draw joint connections ),  e_aabbBit ( draw axis aligned bounding boxes ),
    // e_pairBit ( draw broad-phase pairs ),   e_centerOfMassBit ( draw a marker at body CoM ).
    game->dbgdrw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit | b2Draw::e_centerOfMassBit | b2Draw::e_particleBit
                           | b2Draw::e_jointBit | b2Draw::e_pairBit);

	std::cout << "\nCappedFPS " << game->getCappedFrameRate() <<" FixedTime " <<  game->getFixedTime()
              << " MaxSteps " << game->getMaxSteps() << std::endl;

	game->startUpdate();

    delete game;

	return 0;
}

Game::Game(float fixedTimeStep, float spriteFps, float cappedFrameRate, int maxSteps)
 : Updater(fixedTimeStep, cappedFrameRate, maxSteps), running(true),
   camPxOffsetX(0), camPxOffsetY(0), _spriteFps(spriteFps), sprNextFrameTime(0), fixedTimez(fixedTimeStep) {
    // fps avg counter
    iz = 0;
    totalFps = 0.0f;
    avgFps = new float[50]{0};
    // sprite fps timer
    sprTime = Timer();
    sprTime.start();
}

float Game::getFps() {
    float giveFPS = 1000.0f / fpsTimer.stop();
    fpsTimer.start();
    //avg fps
    totalFps -= avgFps[iz];
    avgFps[iz] = giveFPS;
    totalFps += giveFPS;
    ++iz;
    if(iz >= 50)
        iz = 0;
    return giveFPS;
}
