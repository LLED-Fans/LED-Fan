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

                if (history.last() > 2000 * 1000) {
                    // We were paused, clear history
                    history.fill(0);
                }

                lastCheckpoint = i;
                lastCheckpointTime = time;
                timePerCheckpoint = (unsigned long) history.scalesolidMean(0.5f, &trustableRotations);

                isReliable =
                    // Can determine some kind of mean
                    trustableRotations >= 2
                    // Rotation is sensible
                    && timePerCheckpoint < 2000 * 1000 && timePerCheckpoint > 5 * 1000;
            }
        }

        isReliable &= sensorSwitch->isReliable;
    }
}

float RotationSensor::estimatedRotation(unsigned long time) const {
    if (!isReliable)
        return -1;

    float rawRotation = (float(lastCheckpoint) / switches.size())
                        + (float) (time - lastCheckpointTime) / (float) timePerCheckpoint;

    if (rawRotation > 3.5) {
        // Missed 3, this is not secure at all
        return -1;
    }

    return std::fmod(rawRotation, 1.0f);
}

int RotationSensor::rotationsPerSecond() {
    return 1000 * 1000 / (timePerCheckpoint * switches.size());
}
