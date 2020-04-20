//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_INTERRUPTGPIOSWITCH_H
#define LED_FAN_INTERRUPTGPIOSWITCH_H

#include <vector>
#include "GPIOVisitor.h"

class InterruptGPIOSwitch : public GPIOVisitor {
public:
    std::vector<int> pins;

    InterruptGPIOSwitch(const std::vector<int> &pins);

    void attach();

    void update(int *visitCheckpoint, unsigned long *visitTime) override;

    String stateDescription() override {
        return "[Interrupt]";
    }

    void registerCheckpoint(int checkpoint);
};


#endif //LED_FAN_INTERRUPTGPIOSWITCH_H
