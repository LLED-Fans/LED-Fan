//
// Created by Lukas Tenbrink on 17.04.20.
//

#ifndef LED_FAN_SPEEDCONTROL_H
#define LED_FAN_SPEEDCONTROL_H


static const float SPEED_CONSTANT_THRESHOLD = 0.0001f;

#include <sensor/RotationSensor.h>
#include <util/PWMPin.h>

class SpeedControl {
public:
    PWMPin *forwardPin;
    PWMPin *backwardPin;

    RotationSensor *rotationSensor;

    float maxSpeedRotationsPerSecond = 0;

    SpeedControl(PWMPin *forwardPin, PWMPin *backwardPin, RotationSensor *rotationSensor, float maxSpeedRotationsPerSecond);

    void setDesiredSpeed(float speed);
    float getDesiredSpeed();

    void update();

private:
    float desiredSpeed = 0;
    float speed = NAN;

    void setSpeed(float speed);
};


#endif //LED_FAN_SPEEDCONTROL_H
