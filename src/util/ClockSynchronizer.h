//
// Created by Lukas Tenbrink on 19.02.20.
//

#ifndef LED_FAN_CLOCKSYNCHRONIZER_H
#define LED_FAN_CLOCKSYNCHRONIZER_H

#include "IntRoller.h"

class ClockSynchronizer {
public:
    unsigned long lastSyncTimestamp;
    unsigned long microsecondsPerFrame;
    IntRoller *frameTimeHistory;

    ClockSynchronizer(unsigned long microsecondsPerFrame, int historyLength);

    unsigned long sync();
};


#endif //LED_FAN_CLOCKSYNCHRONIZER_H
