#include "timer.h"

//SDL_TICKS_PASSED(SDL_GetTicks(), gameTime)
Timer::Timer() {
    ticker = 0;
}

Timer::~Timer() {
    //delete[] timeContainer;
}

void Timer::start() {
    ticker = clock();
}

size_t Timer::stop() {
    ticker = clock() - ticker;
    return ticker;
}

float Timer::getSeconds() {
    return ticker/static_cast<float>(CLOCKS_PER_SEC);
}



