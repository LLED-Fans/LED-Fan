//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_SYNCGPIOSWITCH_H
#define LED_FAN_SYNCGPIOSWITCH_H


#include "GPIOVisitor.h"
#include "SensorSwitch.h"

#include <vector>

class SyncGPIOSwitch : public GPIOVisitor {
public:
    std::vector<SensorSwitch *> switches;

    SyncGPIOSwitch(const std::vector<int>& pins, double decay);

    void update(int *visitCheckpoint, unsigned long *visitTime) override;

    String stateDescription() override {
        return String(switches[0]->peaks->lower)
               + " < " + String(switches[0]->rawValue())
               + " < " + String(switches[0]->peaks->upper);
    }
};


#endif //LED_FAN_SYNCGPIOSWITCH_H
