//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_INTERRUPTGPIOSWITCH_H
#define LED_FAN_INTERRUPTGPIOSWITCH_H

#include <vector>
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/c++/5.2.0/vector"
#include "GPIOVisitor.h"

class InterruptGPIOSwitch : public GPIOVisitor {
public:
    std::vector<int> pins;

    int lastCheckpoint;
    unsigned long lastVisitTime;

    InterruptGPIOSwitch(const std::vector<int> &pins);

    void attach();

    void update(unsigned long micros, int *visitCheckpoint, unsigned long *visitTime) override;

    String stateDescription() override {
        return "<Interrupt>";
    }
};


#endif //LED_FAN_INTERRUPTGPIOSWITCH_H
