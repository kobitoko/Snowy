#pragma once
#include <ctime>
#include <iostream>
class Timer {
public:
    Timer();
    ~Timer();
    void start();
    size_t stop();
    float getSeconds();

private:
    clock_t ticker;
};
