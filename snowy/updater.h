#pragma once
#include "tools.h"

class Updater {
public:

    /// default fixed step is 60Hz, frame rate is capped by default at 500fps,
    /// maxSteps prevents spiral of death default is 8 loops.
    Updater(float fixedTimeStep = 1.0f/60.0f, float cappedFrameRate = 1.0f/500.0f, int maxSteps = 8);
    Updater(const Updater& rhs);
    virtual ~Updater() {}

    /// Update based on framerate that is at most cappedFrameRate fast (basically as fast as possible).
    virtual void update() =0;

    /// Fixed update where things like physics should happen.
    virtual void fixedUpdate() =0;

    /// start to update this updater, thus starting an internal loop.
    void startUpdate();

    /// stops to update this updater, stopping the internal loop.
    void stopUpdate();

    /// returns whether the updater's internal loop is running or not.
    bool isRunning() const { return running; }

    /// returns the fixed time step Hz
    float getFixedTime() const { return _fixedTimeStep; }

    /// returns this updater's maximal loop to prevent spiral of death.
    int getMaxSteps() const { return _maxSteps; }

    /// returns the updater's capped off frame rate.
    float getCappedFrameRate() const { return _cappedFrameRate; }

    ///  sets the updater's capped off frame rate, this updater will not updater more than given Hz.
    void setCappedFrameRate(float hz) { _cappedFrameRate = hz; }

    /// returns the updater fixed update's accumulator value (whoese units is in seconds).
    float getAccumulator() const { return accumulator; }

    /// returns the updater fixed update's accumulator ratio.
    float getAccumulatorRatio() const { return accumulatorRatio; }

private:
    // prevent usage of copy construct and assignment operator
    Updater& operator=(const Updater& rhs){}

    // fixed time step from http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
    void eternalLoop();

    float _fixedTimeStep;
    float _cappedFrameRate;
	int _maxSteps;
	Timer timer;
	bool running;
	float renderTime;
	float accumulator;
	float accumulatorRatio;

};
