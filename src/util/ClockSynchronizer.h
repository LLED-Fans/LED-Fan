//
// Created by Lukas Tenbrink on 19.02.20.
//

#ifndef LED_FAN_CLOCKSYNCHRONIZER_H
#define LED_FAN_CLOCKSYNCHRONIZER_H


class ClockSynchronizer {
public:
    unsigned long lastSyncTimestamp;
    unsigned long microsecondsPerFrame;

    ClockSynchronizer(unsigned long microsecondsPerFrame);

    unsigned long sync();
};


#endif //LED_FAN_CLOCKSYNCHRONIZER_H
