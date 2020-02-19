//
// Created by Lukas Tenbrink on 19.02.20.
//

#include "ClockSynchronizer.h"
#include <HardwareSerial.h>
#include <climits>

ClockSynchronizer::ClockSynchronizer(unsigned long microsecondsPerFrame)
: lastSyncTimestamp(INT_MAX), microsecondsPerFrame(microsecondsPerFrame) {}

unsigned long ClockSynchronizer::sync() {
    unsigned long microseconds = micros();

    if (lastSyncTimestamp == INT_MAX) {
        // First call
        lastSyncTimestamp = microseconds;
        return lastSyncTimestamp;
    }

    unsigned long frameTime = (microseconds - lastSyncTimestamp);

    if (microsecondsPerFrame > frameTime) {
        delayMicroseconds(microsecondsPerFrame - frameTime);
    }
    // Else we can't keep up! Lower framerate.

    lastSyncTimestamp = microseconds + frameTime;
    return lastSyncTimestamp;
}
