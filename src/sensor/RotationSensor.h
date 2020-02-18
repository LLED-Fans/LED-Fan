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
    unsigned long lastCheckpointMillis;
    IntRoller history = IntRoller(5);

    unsigned long timePerCheckpoint;
    float rotation;
    bool isReliable;

    RotationSensor(const std::vector<SensorSwitch*> & switches);

    void update(unsigned long currentTime);

private:
    int trustableRotations;
};


#endif //LED_FAN_ROTATIONSENSOR_H
