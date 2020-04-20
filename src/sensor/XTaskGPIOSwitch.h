//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_XTASKGPIOSWITCH_H
#define LED_FAN_XTASKGPIOSWITCH_H


#include <util/XTaskTimer.h>
#include "SyncGPIOSwitch.h"

class XTaskGPIOSwitch: public SyncGPIOSwitch {
public:
    int lastCheckpoint;
    unsigned long lastVisitTime;

    XTaskTimer *timer;

    XTaskGPIOSwitch(const std::vector<int> &pins, double decay, int delay);

    void update(int *visitCheckpoint, unsigned long *visitTime) override;
};


#endif //LED_FAN_XTASKGPIOSWITCH_H
