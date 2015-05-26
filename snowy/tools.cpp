#include "tools.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

std::string randStr() {
    std::ostringstream rnd;
    rnd << rand();
    return rnd.str();
}

void callError(std::string msg, bool quit) {
	// log the time
	time_t timer = time(nullptr);
	// redirect stderr into a log file
	std::freopen("error.log", "a", stderr);
	// write the error.
	std::cerr << ctime(&timer);
	std::cerr << msg << std::endl;
	std::cerr << std::endl;
	// exit program if needed
	if(quit) {
		exit(EXIT_FAILURE);
	}
}

// Timer class

Timer::Timer() {
    ticker = 0;
    clkPs = 1.0f / static_cast<float>(CLOCKS_PER_SEC);
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

float Timer::getSeconds() {
    return lastTime * clkPs;
}



