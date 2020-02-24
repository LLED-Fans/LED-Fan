//
// Created by Lukas Tenbrink on 19.02.20.
//

#include "ClockSynchronizer.h"
#include <HardwareSerial.h>
#include <climits>

ClockSynchronizer::ClockSynchronizer(unsigned long microsecondsPerFrame, int historyLength)
: lastSyncTimestamp(INT_MAX), microsecondsPerFrame(microsecondsPerFrame), frameTimeHistory(new IntRoller(historyLength)) {}

unsigned long ClockSynchronizer::sync() {
    unsigned long microseconds = micros();

    if (lastSyncTimestamp == INT_MAX) {
        // First call
        lastSyncTimestamp = microseconds;
        return lastSyncTimestamp;
    }

    unsigned long frameTime = (microseconds - lastSyncTimestamp);
    frameTimeHistory->push(frameTime);

    if (microsecondsPerFrame > frameTime) {
        delayMicroseconds(microsecondsPerFrame - frameTime);
        lastSyncTimestamp = micros();
    }
    else
        // Can't keep up! Accept lower framerate and just continue running.
        lastSyncTimestamp = microseconds;

    return lastSyncTimestamp;
}
