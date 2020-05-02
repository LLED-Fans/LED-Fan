//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_ROTATIONSENSOR_H
#define LED_FAN_ROTATIONSENSOR_H


#include <util/IntRoller.h>
#include "SensorSwitch.h"
#include "GPIOVisitor.h"
#include <vector>
#include <util/extrapolation/Extrapolator.h>

class RotationSensor {
public:
    IntRoller *checkpointTimestamps;
    IntRoller *checkpointIndices;
    int minCheckpointPasses = 2;
    unsigned long minCheckpointTime = 10 * 1000;
    unsigned long pauseInterval = 2000 * 1000;
    bool separateCheckpoints = false;

    GPIOVisitor *visitor;

    // x = micros, y = rotation 0 to switches.count
    Extrapolator *extrapolator;
    bool isPaused = false;

    // Tool to fix rotation to a specific value
    float fixedRotation = -1;

    // Whether the last frame was a checkpoint frame
    bool didUpdate = false;

    RotationSensor(GPIOVisitor *visitor, int historySize, Extrapolator *extrapolator);

    void update();
    void registerCheckpoint(unsigned long time, int checkpoint);

    // Returns a value from 0 to 1, or NAN if unreliable
    float estimatedRotation(unsigned long time);
    bool isReliable();

    float rotationsPerSecond();
    String stateDescription();

private:
    bool _isReliable = false;
    int estimatedDirection();
};


#endif //LED_FAN_ROTATIONSENSOR_H
