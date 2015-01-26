#include "errorHandler.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

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
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
}
