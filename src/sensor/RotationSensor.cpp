//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include <util/Logger.h>
#include "RotationSensor.h"

#include "Setup.h"

RotationSensor::RotationSensor(GPIOVisitor *visitor, int historySize, int minCheckpointPasses, unsigned long  minCheckpointTime, unsigned long  maxCheckpointTime, Extrapolator *extrapolator) :
    checkpointTimestamps(new IntRoller(historySize)),
    checkpointIndices(new IntRoller(historySize)),
    minCheckpointPasses(minCheckpointPasses),
    minCheckpointTime(minCheckpointTime),
    maxCheckpointTime(maxCheckpointTime),
    visitor(visitor),
    extrapolator(extrapolator)  {
}

void RotationSensor::update(unsigned long time) {
    int checkpoint = -1;
    visitor->update(time, &checkpoint, &time);

    unsigned long checkpointTime = time - checkpointTimestamps->last();

    if (checkpoint >= 0 && checkpointTime >= minCheckpointTime) {
        if (checkpointTime > maxCheckpointTime) {
            // We were paused, clear history first
            // TODO Will also happen when time rolls over, but eh

            checkpointTimestamps->fill(0);
            checkpointIndices->fill(-1);
            isReliable = false;

            // But do register the new checkpoint
        }

        registerCheckpoint(time, checkpoint);
    }
}

void RotationSensor::registerCheckpoint(unsigned long time, int checkpoint) {
    unsigned int historySize = checkpointIndices->count;
    unsigned int checkpointCount = visitor->checkpointCount;

    checkpointIndices->append(checkpoint);
    checkpointTimestamps->append(time);

    int n = (int) historySize - checkpointIndices->countOccurrences(-1);

    if (n < minCheckpointPasses) {
        // Too little data to make meaningful extrapolation
        isReliable = false;
        return;
    }

    // Raw Data Collection
    std::vector<double> x{};
    x.reserve(n);
    std::vector<int> estimatedY{};
    estimatedY.reserve(n);

    for (int j = 0; j < historySize; ++j) {
        int checkpointIndex = (*checkpointIndices)[j];

        if (checkpointIndex < 0)
            continue; // Not set yet

        x.push_back((*checkpointTimestamps)[j]);
        estimatedY.push_back(checkpointIndex);
    }

    double estimatedCheckpointDiff = INT_MAX;
    for (int j = 1; j < n; ++j) {
        int xDiff = (*checkpointTimestamps)[j] - (*checkpointTimestamps)[j - 1];
        if (xDiff < estimatedCheckpointDiff)
            estimatedCheckpointDiff = xDiff;
    }

    // Try to estimate if we missed any checkpoints
    std::vector<double> y(n);

    if (estimatedCheckpointDiff <= 0) {
        // Not sure what happened... but don't take the chance.
        Logger::println("xDiffMean = 0; unable to sync rotation...");
        isReliable = false;
        return;
    }

    // Go back in reverse, setting reached checkpoint as baseline Y
    y[n - 1] = estimatedY[n - 1];
    for (int j = n - 2; j >= 0; --j) {
        unsigned int expectedSteps = (estimatedY[j + 1] - estimatedY[j] + checkpointCount) % checkpointCount;
        double estimatedSteps = (x[j + 1] - x[j]) / estimatedCheckpointDiff;

        // Accept +- multiples of checkpointCount
        y[j] = y[j + 1] - round((estimatedSteps - expectedSteps) / checkpointCount) * checkpointCount + expectedSteps;
    }

    extrapolator->adjust(x, y);
    double rotationsPerSecond = this->rotationsPerSecond();

    // Speed is sensible?
    isReliable = rotationsPerSecond < 100 && rotationsPerSecond > 1;
}

float RotationSensor::estimatedRotation(unsigned long time) {
    if (!isReliable)
        return NAN;

    float rawRotation = extrapolator->extrapolate(time);

    if (std::isnan(rawRotation) || rawRotation > 3.5) {
        // Missed 3 checkpoints, this is not secure at all
        return NAN;
    }

    return std::fmod(rawRotation / (float) visitor->checkpointCount, 1.0f);
}

int RotationSensor::rotationsPerSecond() {
    return (int) (extrapolator->slope() * 1000 * 1000);
}

String RotationSensor::stateDescription() {
    return visitor->stateDescription();
}
