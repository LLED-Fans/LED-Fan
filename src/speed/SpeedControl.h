//
// Created by Lukas Tenbrink on 17.04.20.
//

#ifndef LED_FAN_SPEEDCONTROL_H
#define LED_FAN_SPEEDCONTROL_H


#include <sensor/RotationSensor.h>

class SpeedControl {
public:
    float maxSpeedRotationsPerSecond = 0;

    RotationSensor *rotationSensor;

    SpeedControl(RotationSensor *rotationSensor, float maxSpeedRotationsPerSecond);

    void setDesiredSpeed(float speed);

    void update();
private:
    float desiredSpeed = 0;
    float speed = 0;

    void setSpeed(float speed);
};


#endif //LED_FAN_SPEEDCONTROL_H
