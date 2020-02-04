//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include "RotationSensor.h"

RotationSensor::RotationSensor(SensorSwitch *sensorSwitch) : sensorSwitch(sensorSwitch) {

}

void RotationSensor::update(unsigned long currentTime) {
    // Test the switch
    if (sensorSwitch->test() && sensorSwitch->isReliable) {
        if (!sensorSwitch->isOn()) {
            // Full Rotation
            rotationHistory.append((int) (currentTime - lastRotationMillis));

            lastRotationMillis = currentTime;
            timePerRotation = (unsigned long) rotationHistory.solidMean(0.5f, &trustableRotations);

            isReliable = trustableRotations >= 2 && timePerRotation < 2000;
            //Printf::ln("Rotation! Took %f", timePerRotation);
        }
        //Printf::ln("Turn! %d", magnetSwitch.isOn);
    }

    float rawRotation = (float) (currentTime - lastRotationMillis) / (float) timePerRotation;

    isReliable &= rawRotation < 4 && sensorSwitch->isReliable;
    rotation = std::fmod(rotation, 1.0f);
}
