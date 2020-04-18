//
// Created by Lukas Tenbrink on 17.04.20.
//

#include <util/Math.h>
#include <cmath>
#include <esp32-hal-gpio.h>
#include "SpeedControl.h"

SpeedControl::SpeedControl(
        PWMPin *forwardPin, PWMPin *backwardPin,
        RotationSensor *rotationSensor, float maxSpeedRotationsPerSecond
):
forwardPin(forwardPin), backwardPin(backwardPin),
rotationSensor(rotationSensor), maxSpeedRotationsPerSecond(maxSpeedRotationsPerSecond) {
    setSpeed(0.0f);
}

void SpeedControl::setDesiredSpeed(float speed) {
    speed = fminf(1.0f, fmaxf(-1.0f, speed));
    desiredSpeed = speed;
    update();
}

float SpeedControl::getDesiredSpeed() {
    return desiredSpeed;
}

void SpeedControl::update() {
    if (desiredSpeed == speed)
        return; // No need to recompute

    float estimatedSpeed = rotationSensor->rotationsPerSecond() / maxSpeedRotationsPerSecond;

    float direction = signum(desiredSpeed);

    float desiredSpeedP = desiredSpeed * direction;
    float estimatedSpeedP = estimatedSpeed * direction;

    if (std::abs(estimatedSpeed) > 0.2f && estimatedSpeedP < 0) {
        // We're spinning fast and want to change direction
        // Brake at full power
        // Don't dare to accelerate in the other direction yet,
        // since it will put awkward forces on the motor.
        setSpeed(0);
        return;
    }

    if (desiredSpeedP > estimatedSpeedP) {
        // Accelerate
        // ... but not too strongly
        setSpeed(fminf(estimatedSpeedP + 0.5f, desiredSpeedP) * direction);
    }
    else {
        // Decelerate
        // Can do this instantly
        setSpeed(desiredSpeed);
    }
}

void SpeedControl::setSpeed(float speed) {
    speed = fminf(1.0f, fmaxf(-1.0f, speed));

    if (this->speed == speed)
        return; // No Change

    this->speed = speed;
    int direction = signum(speed);

    if (speed == 0 || std::abs(speed) == 1) {
        forwardPin->setConstant(0);
        backwardPin->setConstant(0);

        // Constant speed, don't need PWM
        return;
    }

    auto lowChannel = direction == 1 ? forwardPin : backwardPin;
    auto highChannel = direction == 1 ? backwardPin : forwardPin;

    lowChannel->setConstant(LOW);
    highChannel->setRatio(std::abs(speed));
}
