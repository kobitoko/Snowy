#pragma once
#include <iostream>

// print the error into a file error.log and quit program unless set false.
void callError(std::string msg, bool quit = true);
