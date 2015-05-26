#pragma once
#include <iostream>
#include <vector>
#include "../snowy/snowy.h"

class Game : public Updater {
public:
    Game(){}
    Game(float fixedTimeStep, float spriteFps, float cappedFrameRate, int maxSteps);
    ~Game();

    float getFps();

    void update();
    void fixedUpdate();
    void updateLogic();

    // tests
    void testInput();
    void loadMySprites();
    void loadMySounds();
    void loadMyPhysics();
    void movePlayer();
    void collisionTesting();

    void loadMyParticles();
    void loadMyJelly();
    void testParticles();

    // objects
    Screen* scr;
    Input* in;
    Sound* snd;
    Physics* phy;
    Camera* cam;

    DebugDraw* dbgdrw;
    ObjectManager* objMan;
    Timer fpsTimer;

    // a container
    std::vector<std::string>bucket;

    // metric units to pixel scale and the reverse
    const float MTOPX = 10.0f;
    const float PXTOM = 1.0f / MTOPX;
    //9.8066 is earth's gravity.
    const float GRAVITY = 40.0f;

    float fixedTimez;

    int w, h;
    bool running;

    int camPxOffsetX;
    int camPxOffsetY;

    // box2D stuff
    b2Body* theBody;

    // for sprite frame updates
    float _spriteFps;
    float sprNextFrameTime;
    Timer sprTime;

    // for average fps count
    int iz;
    float totalFps;
    float *avgFps;

    //file locations
    const char* blobSrc = "example/images/water.png";
    const char* bgSrc = "example/images/bg.png";
    const char* boxSrc = "example/images/box.png";
    const char* meteorSrc = "example/images/meteor.png";
    const char* bgm = "example/sounds/Out of place NES.wav";
    const char* sfx = "example/sounds/stream5.ogg";
    const char* fontLoc = "example/kenpixel_mini_square.ttf";
    const char* tileLoc = "example/images/tileSetKennyNL.png";
};
