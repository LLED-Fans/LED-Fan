//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include "RotationSensor.h"

RotationSensor::RotationSensor(const std::vector<SensorSwitch*> & switches) : switches(switches) {

}

void RotationSensor::update(unsigned long currentTime) {
    for (int i = 0; i < switches.size(); ++i) {
        SensorSwitch *sensorSwitch = switches[i];

        // Test the switch
        if (sensorSwitch->test() && sensorSwitch->isReliable) {
            if (!sensorSwitch->isOn()) {
                // Full Rotation
                history.append((int) (currentTime - lastCheckpointMillis));

                lastCheckpoint = i;
                lastCheckpointMillis = currentTime;
                timePerCheckpoint = (unsigned long) history.solidMean(0.5f, &trustableRotations);

                isReliable = trustableRotations >= 2 && timePerCheckpoint < 2000 && timePerCheckpoint > 5;
            }
        }

        float rawRotation = (float(lastCheckpoint) / switches.size())
                + (float) (currentTime - lastCheckpointMillis) / (float) timePerCheckpoint;

        isReliable &= rawRotation < 4 && sensorSwitch->isReliable;
        rotation = std::fmod(rawRotation, 1.0f);
    }
}
