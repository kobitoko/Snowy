#pragma once

#include <iostream>
#include <vector>
#include "../snowy/snowy.h"

void updateLogic();
float getFps();

// tests
bool testInput();
void loadMySprites();
void loadMySounds();
b2Body* loadMyPhysics();
void testPhysics(b2Body* body);
void loadMyParticles();
void loadMyJelly();
void testParticles();


// objects
Screen scr;
Input in;
Sound snd;
Physics phy;
Timer fpsTimer;
ObjectManager objMan;

// a container
std::vector<std::string>bucket;

// metric units to pixel scale
const float MTOPX = 10;
//9.8066 is earth's gravity.
const float GRAVITY = 40.0f;;
int w, h;
bool running;
// core game logic updates will 60Hz
const float FIXED_TIMESTEP = 1.0f / 60.0f;
// fps capped at 60fps
const float CAPPED_FRAMERATE = 1.0f / 60.0f;
float accumulator, accumulatorRatio;

//test
b2Body* theBody;

//file locations
const char* blobSrc = "example/images/water.png";
const char* bgSrc = "example/images/bg.png";
const char* boxSrc = "example/images/box.png";
const char* meteorSrc = "example/images/meteor.png";
const char* bgm = "example/sounds/Out of place NES.wav";
const char* sfx = "example/sounds/stream5.ogg";
const char* fontLoc = "example/kenpixel_mini_square.ttf";
const char* tileLoc = "example/images/tileSetKennyNL.png";
