//
// Created by Lukas Tenbrink on 17.04.20.
//

#ifndef LED_FAN_SPEEDCONTROL_H
#define LED_FAN_SPEEDCONTROL_H


#include <sensor/RotationSensor.h>

class SpeedControl {
public:
    RotationSensor *rotationSensor;

    float maxSpeedRotationsPerSecond = 0;

    SpeedControl(RotationSensor *rotationSensor, float maxSpeedRotationsPerSecond);

    void setDesiredSpeed(float speed);
    float getDesiredSpeed();

    void update();

private:
    float desiredSpeed = 0;
    float speed = 0;

    void setSpeed(float speed);
};


#endif //LED_FAN_SPEEDCONTROL_H
