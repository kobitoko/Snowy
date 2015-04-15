#pragma once
#include <iostream>
#include <sstream>

// print the error into a file error.log and quit program unless set false.
void callError(std::string msg, bool quit = true);

// provide a function to convert to string so one can print out values that cause errors.
template <typename T> std::string toStr(T in) {
    std::ostringstream o;
    o << in;
    return o.str();
}
