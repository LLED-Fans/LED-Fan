//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_ROTATIONSENSOR_H
#define LED_FAN_ROTATIONSENSOR_H


#include <util/IntRoller.h>
#include "SensorSwitch.h"
#include <list>

class RotationSensor {
public:
    std::vector<SensorSwitch*> switches;

    unsigned int lastCheckpoint;
    unsigned long lastCheckpointTime;
    IntRoller history = IntRoller(5);

    unsigned long timePerCheckpoint;
    bool isReliable;

    RotationSensor(const std::vector<SensorSwitch*> & switches);

    void update(unsigned long time);

    // Returns a value from 0 to 1
    // -1 if rotation could not be estimated successfully
    float estimatedRotation(unsigned long time) const;

    int rotationsPerSecond();
private:
    int trustableRotations;
};


#endif //LED_FAN_ROTATIONSENSOR_H
