//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <cmath>
#include <util/Logger.h>
#include <util/cluster/FastCluster.h>
#include <util/Math.h>
#include "RotationSensor.h"

#include "Setup.h"

RotationSensor::RotationSensor(GPIOVisitor *visitor, int historySize, Extrapolator *extrapolator) :
    checkpointTimestamps(new IntRoller(historySize)),
    checkpointIndices(new IntRoller(historySize)),
    visitor(visitor),
    extrapolator(extrapolator)  {
}

void RotationSensor::update(unsigned long time) {
    int checkpoint = -1;
    visitor->update(time, &checkpoint, &time);

    if (checkpoint < 0) {
        // TODO Handle when time rolls over
        if (isPaused || (time - checkpointTimestamps->last()) < pauseInterval) {
            return;
        }

        checkpointTimestamps->fill(0);
        checkpointIndices->fill(-1);
        isReliable = true;
        isPaused = true;

        return;
    }

    unsigned long checkpointTime = time - checkpointTimestamps->last();

    int checkpointCount = visitor->checkpointCount;
    // 0->1 = 1; 0->0 = count
    int elapsedCheckpoints = ((checkpoint - checkpointIndices->last()) + checkpointCount - 1) % checkpointCount + 1;

    if (checkpointTime < minCheckpointTime * elapsedCheckpoints) {
        return;
    }

    registerCheckpoint(time, checkpoint);
}

void RotationSensor::registerCheckpoint(unsigned long time, int checkpoint) {
    int historySize = checkpointIndices->count;
    int checkpointCount = visitor->checkpointCount;

    checkpointIndices->push(checkpoint);
    checkpointTimestamps->push((int) time);

    int n = (int) historySize - checkpointIndices->countOccurrences(-1);

    if (n < minCheckpointPasses) {
        // Too little data to make meaningful extrapolation
        isReliable = false;
        return;
    }

    // If we have <= 2 checkpoints, can't estimate direction. Just assume 1.
    auto direction = historySize > 2 ? estimatedDirection() : 1;
    if (direction == 0) {
        // Unsure about direction - probably unreliable!
        isReliable = false;
        return;
    }
    // -1 if backwards
    auto backwardsAdjust = (direction - 1) / 2;

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

    double estimatedCheckpointDiff = FastCluster::center(
        FastCluster::stepDiffs(x),
        10 * 1000
    );

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
        int expectedSteps = (estimatedY[j + 1] - estimatedY[j] + checkpointCount) % checkpointCount + backwardsAdjust;
        double estimatedSteps = (x[j + 1] - x[j]) / estimatedCheckpointDiff;

        // Accept +- multiples of checkpointCount
        y[j] = y[j + 1] - (round((estimatedSteps - expectedSteps) / checkpointCount) * checkpointCount + expectedSteps);
    }

    if (separateCheckpoints) {
        // Expand any available checkpoint -> (checkpoint + 1) to full rotation
        // -- Practically 'deleting' any other segments from time history
        // -- Regress, and then 'unexpand' the segment into its intended size
        int segmentStart = checkpoint;

        // Always keep last x and y, since it's our current reference
        for (int i = n - 2; i >= 0; --i) {
            if (i == 0 || std::lround(estimatedY[i - 1]) != segmentStart || (std::lround(y[i] - y[i - 1]) != 1)) {
                // Uninteresting; remove!
                x.erase(x.begin() + i);
                y.erase(y.begin() + i);
                continue;
            }

            // Found a separatable segment, determine its length
            // i + 1 is our last reference, sync the current segment up to it, deleting any time in between
            x[i] = x[i + 1] - (x[i] - x[i - 1]);
            // Since we're only considering in-segment differences, set y to reflect 1 segment slopes
            y[i] = y[i + 1] - 1;
        }

        // Best case we now have n / checkpointCount references
        if (x.size() < minCheckpointPasses) {
            isReliable = false;
            return;
        }
    }

    extrapolator->adjust(x, y);
    double rotationsPerSecond = this->rotationsPerSecond();

    // Speed is sensible?
    isReliable = rotationsPerSecond < 100 && rotationsPerSecond > 1;
}

int RotationSensor::estimatedDirection() {
    int historySize = checkpointIndices->count;

    int direction = 0;
    for (unsigned int i = checkpointIndices->count; i > 1; i++) {
        int diff = (((*checkpointIndices)[i] == (*checkpointIndices)[i - 1]) + historySize) % historySize;
        if (diff == 0)
            continue;

        if (diff < historySize / 2)
            direction++; // Moved < 180°; probably forwards
        else if (diff > historySize / 2) {
            direction--; // Moved > 180°; probably backwards
        }
    }
    return signum(direction);
}

float RotationSensor::estimatedRotation(unsigned long time) {
    if (!isReliable || isPaused)
        return NAN;

    float position = extrapolator->extrapolate(time);

    if (std::isnan(position))
        return NAN;

    float rotation = position / (float) visitor->checkpointCount;

    if (rotation > 3.5 || rotation < -3.5)
        // Missed checkpoints for > 3 rotations...
        // Or went back too far
        return NAN;

    // Add 10 beforehand so we're always positive
    return std::fmod(rotation + 10, 1.0f);
}

int RotationSensor::rotationsPerSecond() {
    if (isPaused)
        return 0;

    return (int) (extrapolator->slope() * 1000 * 1000);
}

String RotationSensor::stateDescription() {
    return visitor->stateDescription();
}
