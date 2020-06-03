//
// Created by Lukas Tenbrink on 17.04.20.
//

#include <util/Math.h>
#include <cmath>
#include <esp32-hal-gpio.h>
#include <Setup.h>
#include <HardwareSerial.h>
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
    flush();
}

void SpeedControl::update(unsigned long microsDelay) {
#ifdef EMERGENCY_BRAKE_ENABLED
    if (speed != 0) {
        if (microsAccelerating < ROTATION_PAUSED_MS * 1000)
            microsAccelerating += microsDelay;
        else if (rotationSensor->isPaused) {
            Serial.println("Motor appears to be stuck - Emergency brake engaged!");
            setDesiredSpeed(0);
            return;
        }
    }
#endif

    if (desiredSpeed == speed)
        return; // No need to recompute

    if (microsDelay < microsUntilUpdate) {
        microsUntilUpdate -= microsDelay;
        return;
    }

    flush();
}

void SpeedControl::flush() {
    microsUntilUpdate = microsPerUpdate;

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

    if (accelerationLimit <= 0) {
        setSpeed(desiredSpeed);
        return;
    }

    if (desiredSpeedP > estimatedSpeedP) {
        // Accelerate
        // ... but not too strongly
        setSpeed(fminf(estimatedSpeedP + accelerationLimit, desiredSpeedP) * direction);
    }
    else {
        // Decelerate
        // Can do this instantly
        setSpeed(desiredSpeed);
    }
}

void SpeedControl::setSpeed(float speed) {
    if (speed == 0)
        microsAccelerating = 0;

    speed = fminf(1.0f, fmaxf(-1.0f, speed));

    if (this->speed == speed)
        return; // No Change

    this->speed = speed;
    int direction = signum(speed);
    float speedRatio = std::abs(speed);

    auto lowChannel = direction == 1 ? forwardPin : backwardPin;
    auto highChannel = direction == 1 ? backwardPin : forwardPin;

    lowChannel->setConstant(LOW);
    highChannel->setRatio(speedRatio);
}
