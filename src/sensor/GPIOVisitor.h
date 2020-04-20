//
// Created by Lukas Tenbrink on 22.02.20.
//

#ifndef LED_FAN_GPIOVISITOR_H
#define LED_FAN_GPIOVISITOR_H

#include <WString.h>

class GPIOVisitor {
public:
    int checkpointCount;

    virtual void update(int *visitCheckpoint, unsigned long *visitTime) = 0;

    virtual String stateDescription() = 0;
};


#endif //LED_FAN_GPIOVISITOR_H
