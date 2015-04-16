#pragma once

#include <iostream>
#include <vector>
#include "../snowy/snowy.h"
#include "../snowy/timer.h"

// tests
bool testInput();
void loadMySprites();
void loadMySounds();
b2Body* loadMyPhysics();
void testPhysics(b2Body* body);
void loadMyParticles();
void testParticles();

float getFps();

// objects
Screen scr;
Input in;
Sound snd;
Physics phy;
Timer timer;

// a container
std::vector<std::string>bucket;

// metric units to pixel scale
const float MTOPX = 10;
int w, h;

//file locations
const char* blobSrc = "example/images/water.png";
const char* bgSrc = "example/images/bg.png";
const char* boxSrc = "example/images/box.png";
const char* meteorSrc = "example/images/meteor.png";
const char* bgm = "example/sounds/Out of place NES.wav";
const char* sfx = "example/sounds/stream5.ogg";
const char* fontLoc = "example/kenpixel_mini_square.ttf";
const char* tileLoc = "example/images/tileSetKennyNL.png";
