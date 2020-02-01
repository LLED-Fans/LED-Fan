//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_ROTATIONSENSOR_H
#define LED_FAN_ROTATIONSENSOR_H


#include "SensorSwitch.h"

class RotationSensor {
public:
    SensorSwitch *sensorSwitch;

    unsigned long lastRotationMillis;
    unsigned long timePerRotation;

    RotationSensor(SensorSwitch *sensorSwitch);

    float update(unsigned long currentTime);
    bool isReliable();
};


#endif //LED_FAN_ROTATIONSENSOR_H
