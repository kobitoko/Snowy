#pragma once
#include <ctime>
#include <iostream>
class Timer {
public:
    Timer();
    ~Timer();
    // resets the timer and start it (again).
    void start();
    // returns the time taken in milliseconds.
    size_t stop();
    float getSeconds() {return lastTime/static_cast<float>(CLOCKS_PER_SEC);}

private:
    clock_t ticker, lastTime;
};
