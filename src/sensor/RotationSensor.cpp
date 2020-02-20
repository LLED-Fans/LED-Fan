//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include <utility>
#include <numeric>
#include "RotationSensor.h"

RotationSensor::RotationSensor(std::vector<SensorSwitch*>  switches, int historySize, Extrapolator *extrapolator) :
    switches(std::move(switches)),
    checkpointTimestamps(new IntRoller(historySize)),
    checkpointIndices(new IntRoller(historySize)),
    extrapolator(extrapolator)  { }

void RotationSensor::update(unsigned long time) {
    unsigned int checkpointCount = switches.size();

    for (int i = 0; i < checkpointCount; ++i) {
        SensorSwitch *sensorSwitch = switches[i];

        // Test the switch
        if (sensorSwitch->test() && sensorSwitch->isReliable) {
            if (!sensorSwitch->isOn()) {
                // Full Rotation

                checkpointTimestamps->append(time);
                (*checkpointIndices)[checkpointTimestamps->head] = i;

                if ((*checkpointTimestamps)[0] - (*checkpointTimestamps)[-1] > 2000 * 1000) {
                    // We were paused, clear history
                    checkpointTimestamps->fill(0);
                    checkpointIndices->fill(-1);

                    isReliable = false;
                    continue;
                }

                int n = (int) checkpointIndices->count - checkpointIndices->countOccurrences(-1);

                if (n < 3) {
                    // Too little data to make meaningful extrapolation
                    isReliable = false;
                    continue;
                }

                // Raw Data Collection
                std::vector<double> x(n);
                std::vector<int> estimatedY(n);

                for (int j = 0; j < n; ++j) {
                    int checkpointIndex = (*checkpointIndices)[j];

                    if (checkpointIndex < 0)
                        continue; // Not set yet

                    x.push_back((*checkpointTimestamps)[j]);
                    estimatedY.push_back(checkpointIndex);
                }

                // Try to estimate if we missed any checkpoints
                std::vector<double> y(n);
                double xDiffMean = (x[n - 1] - x[0]) / (x.size() - 1);

                // Go back in reverse, setting reached checkpoint as baseline Y
                y[n - 1] = estimatedY[n - 1];
                for (int j = n - 2; j >= 0; ++j) {
                    unsigned int expectedSteps = (estimatedY[j + 1] - estimatedY[j] + checkpointCount) % checkpointCount;
                    double estimatedSteps = (x[j + 1] - x[j]) / xDiffMean;

                    // Accept +- multiples of checkpointCount
                    y[j] = y[j + 1] - round((estimatedSteps - expectedSteps) / checkpointCount) * checkpointCount + expectedSteps;
                }

                extrapolator->adjust(x, y);
                double rotationsPerSecond = this->rotationsPerSecond();

                // Speed is sensible
                isReliable = rotationsPerSecond < 100 && rotationsPerSecond > 1;
            }
        }

        isReliable &= sensorSwitch->isReliable;
    }
}

float RotationSensor::estimatedRotation(unsigned long time) {
    if (!isReliable)
        return NAN;

    float rawRotation = extrapolator->extrapolate(time);

    if (std::isnan(rawRotation) || rawRotation > 3.5) {
        // Missed 3 checkpoints, this is not secure at all
        return NAN;
    }

    return std::fmod(rawRotation / switches.size(), 1.0f);
}

int RotationSensor::rotationsPerSecond() {
    return (int) (extrapolator->slope() / 1000 / 1000);
}
