//
// Created by Lukas Tenbrink on 19.02.20.
//

#ifndef LED_FAN_REGULARCLOCK_H
#define LED_FAN_REGULARCLOCK_H

#include "IntRoller.h"

class RegularClock {
public:
    unsigned long lastSyncTimestamp;
    unsigned long microsecondsPerFrame;

    unsigned long timeSinceLastSync;
    IntRoller *frameTimeHistory;

    RegularClock(unsigned long microsecondsPerFrame, int historyLength);

    unsigned long sync();
};


#endif //LED_FAN_REGULARCLOCK_H
