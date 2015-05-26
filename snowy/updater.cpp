#include "updater.h"

Updater::Updater(float fixedTimeStep, float cappedFrameRate, int maxSteps)
: _fixedTimeStep(fixedTimeStep), _cappedFrameRate(cappedFrameRate), _maxSteps(maxSteps) {
    timer = Timer();
    running = false;
    renderTime = 0.0f;
    accumulator = 0.0f;
    accumulatorRatio = 0.0f;
}

Updater::Updater(const Updater& rhs)
: _fixedTimeStep(rhs._fixedTimeStep), _cappedFrameRate(rhs._cappedFrameRate), _maxSteps(rhs._maxSteps) {
    timer = rhs.timer;
    running = rhs.running;
    renderTime = rhs.renderTime;
    accumulator = rhs.accumulator;
    accumulatorRatio = rhs.accumulatorRatio;
}

void Updater::startUpdate() {
    running = true;
    eternalLoop();
}

void Updater::stopUpdate() {
    running = false;
}

void Updater::eternalLoop() {
    while(running) {
        timer.stop();
        renderTime += timer.getSeconds();
        accumulator += timer.getSeconds();
        timer.start();
        //const int nSteps = static_cast<int>(std::floor(accumulator / _fixedTimeStep));
        const int nSteps = static_cast<int>(accumulator / _fixedTimeStep);

        //avoiding rounding error, touches accumulator only if needed.
        if(nSteps > 0)
            accumulator -= nSteps * _fixedTimeStep;
        accumulatorRatio = accumulator / _fixedTimeStep;

        // similar to clamp "dt" but keeps above calculations of accumulator and accumulatorRatio unchaged.
        const int nStepsClamped = std::min(nSteps, _maxSteps);
        for(int i = 0; i < nStepsClamped; ++i) {
            // this is fixed update
            fixedUpdate();
        }

        // not fixed update
        if(renderTime >= _cappedFrameRate) {
            renderTime = 0;
            update();
        }
    }
}
