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
    ticker = clock() - ticker;
    return ticker;
}

float Timer::getSeconds() {
    return ticker/static_cast<float>(CLOCKS_PER_SEC);
}



