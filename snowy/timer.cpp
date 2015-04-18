#include "timer.h"

Timer::Timer() {
    ticker = 0;
}

Timer::~Timer() {
}

void Timer::start() {
    ticker = clock();
}

size_t Timer::stop() {
    lastTime = clock() - ticker;
    return lastTime;
}




