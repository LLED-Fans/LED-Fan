//
// Created by Lukas Tenbrink on 19.02.20.
//

#include "RegularClock.h"
#include <HardwareSerial.h>
#include <climits>

RegularClock::RegularClock(unsigned long microsecondsPerFrame, int historyLength)
: lastSyncTimestamp(INT_MAX), microsecondsPerFrame(microsecondsPerFrame), frameTimeHistory(new IntRoller(historyLength)) {}

unsigned long RegularClock::sync() {
    unsigned long microseconds = micros();

    if (lastSyncTimestamp == INT_MAX) {
        // First call
        lastSyncTimestamp = microseconds;
        return lastSyncTimestamp;
    }

    unsigned long frameTime = (microseconds - lastSyncTimestamp);
    frameTimeHistory->push(frameTime);
    auto previousTimestamp = lastSyncTimestamp;

    if (microsecondsPerFrame > frameTime) {
        unsigned long delay = microsecondsPerFrame - frameTime;
        delayMicroseconds(delay);
        // If the delay goes too long or too short, we can use that to calculate our next frame
        lastSyncTimestamp = microseconds + delay;
    }
    else
        // Can't keep up! Accept lower framerate and just continue running.
        lastSyncTimestamp = microseconds;

    return timeSinceLastSync = lastSyncTimestamp - previousTimestamp;
}
