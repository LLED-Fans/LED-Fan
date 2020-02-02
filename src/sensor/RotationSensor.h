//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_ROTATIONSENSOR_H
#define LED_FAN_ROTATIONSENSOR_H


#include <util/IntRoller.h>
#include "SensorSwitch.h"

class RotationSensor {
public:
    SensorSwitch *sensorSwitch;

    unsigned long lastRotationMillis;
    IntRoller rotationHistory = IntRoller(5);

    int timePerRotation;
    float rotation;
    bool isReliable;

    RotationSensor(SensorSwitch *sensorSwitch);

    void update(unsigned long currentTime);

private:
    int trustableRotations;
};


#endif //LED_FAN_ROTATIONSENSOR_H
