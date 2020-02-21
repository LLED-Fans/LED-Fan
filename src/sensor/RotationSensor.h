//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_ROTATIONSENSOR_H
#define LED_FAN_ROTATIONSENSOR_H


#include <util/IntRoller.h>
#include "SensorSwitch.h"
#include <vector>
#include <util/extrapolation/Extrapolator.h>

class RotationSensor {
public:
    std::vector<SensorSwitch*> switches;

    IntRoller *checkpointTimestamps;
    IntRoller *checkpointIndices;
    int minCheckpointPasses;

    // x = micros, y = rotation 0 to switches.count
    Extrapolator *extrapolator;
    bool isReliable;

    RotationSensor(std::vector<SensorSwitch*> switches, int historySize, int minCheckpointPasses, Extrapolator *extrapolator);

    void update(unsigned long time);

    // Returns a value from 0 to 1, or NAN if unreliable
    float estimatedRotation(unsigned long time);

    int rotationsPerSecond();
};


#endif //LED_FAN_ROTATIONSENSOR_H
