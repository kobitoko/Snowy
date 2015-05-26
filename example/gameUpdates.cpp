#include "game.h"

// Update based on framerate that is at most cappedFrameRate fast.
void Game::update() {
    // physics extrapolation
    phy->getWorld()->ClearForces();
    phy->smoothStates(getAccumulatorRatio(), getFixedTime());

    // update camera offset
    cam->camUpdate(objMan->getAllObjects(), scr);

    // render all to-be-rendered sprites
    scr->renderScreen();

    // draw the debug box2d stuff
    //phy->getWorld()->DrawDebugData();

    // update screen.
    scr->showScreen();

    // set all rendered sprite to the next frame if it's time stored in ms.
    sprNextFrameTime += sprTime.stop();
    if(sprNextFrameTime >= _spriteFps * 1000) {
        sprNextFrameTime = 0;
        scr->allRenderSpritesNextFrame();
    }
    sprTime.start();

    float fpsVal = getFps();

    float val = totalFps/50.0f;
    std::cout << "avgFPS:" << val << " rawFPS "<< fpsVal << ".            \r";

    std::ostringstream winNameFps;
    winNameFps << "FPS:" << val;
    std::string winName = "Surf! - " + winNameFps.str();
    SDL_SetWindowTitle(scr->getWindow(), winName.c_str());

    if(!running)
        stopUpdate();
}

// Fixed update where things like physics should happen.
void Game::fixedUpdate() {
    updateLogic();
}

void Game::updateLogic() {
    in->updateInput();

    // Test input
    testInput();
    movePlayer();

    // particle testing
    testParticles();

    // update physics.
    phy->resetSmoothStates();
    phy->update();

    collisionTesting();
}
