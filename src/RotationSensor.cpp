//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "RotationSensor.h"

RotationSensor::RotationSensor(SensorSwitch *sensorSwitch) : sensorSwitch(sensorSwitch) {}

float RotationSensor::update(unsigned long currentTime) {
    // Test the switch
    if (sensorSwitch->test() && sensorSwitch->isReliable) {
        if (!sensorSwitch->isOn()) {
            // Full Rotation
            timePerRotation = currentTime - lastRotationMillis;
            lastRotationMillis = currentTime;
            //printfln("Rotation! Took %f", timePerRotation);
        }
        //printfln("Turn! %d", magnetSwitch.isOn);
    }

    return (float) (currentTime - lastRotationMillis) / (float) timePerRotation;
}
