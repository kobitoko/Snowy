#pragma once
#include <sstream>
#include <ctime>
#include <string>

/// provide a function to convert to string.
template <typename T> std::string toStr(T in);

/// returns a random alphanu
std::string randStr();

/// print the error into a file error.log and quit program unless set false.
void callError(std::string msg, bool quit = true);

/// class for timers that keep track of how much time passed.
class Timer {
public:
    Timer();
    ~Timer();
    /// resets the timer and start it (again).
    void start();
    /// returns the time taken in milliseconds.
    size_t stop();
    /// returns the time taken in seconds.
    float getSeconds();

private:
    clock_t ticker, lastTime;
    float clkPs;
};


// defintions
template <typename T> inline std::string toStr(T in) {
    std::ostringstream o;
    o << in;
    return o.str();
}

