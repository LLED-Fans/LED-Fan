//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include "RotationSensor.h"

RotationSensor::RotationSensor(const std::vector<SensorSwitch*> & switches) : switches(switches) {

}

void RotationSensor::update(unsigned long time) {
    for (int i = 0; i < switches.size(); ++i) {
        SensorSwitch *sensorSwitch = switches[i];

        // Test the switch
        if (sensorSwitch->test() && sensorSwitch->isReliable) {
            if (!sensorSwitch->isOn()) {
                // Full Rotation
                history.append((int) (time - lastCheckpointTime));

                lastCheckpoint = i;
                lastCheckpointTime = time;
                timePerCheckpoint = (unsigned long) history.solidMean(0.5f, &trustableRotations);

                isReliable = trustableRotations >= 2 && timePerCheckpoint < 2000 * 1000 && timePerCheckpoint > 5 * 1000;
            }
        }

        float rawRotation = (float(lastCheckpoint) / switches.size())
                + (float) (time - lastCheckpointTime) / (float) timePerCheckpoint;

        isReliable &= rawRotation < 4 && sensorSwitch->isReliable;
        rotation = std::fmod(rawRotation, 1.0f);
    }
}

int RotationSensor::rotationsPerSecond() {
    return 1000 * 1000 / (timePerCheckpoint * switches.size());
}
