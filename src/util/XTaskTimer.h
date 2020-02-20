//
// Created by Lukas Tenbrink on 19.02.20.
//

#ifndef LED_FAN_XTASKTIMER_H
#define LED_FAN_XTASKTIMER_H


#include "IntRoller.h"
#include <cstddef>
#include <functional>
#include <WString.h>

#include <esp32-hal.h>

class XTaskTimer {
public:
    TaskHandle_t handle = NULL;
    TickType_t frameTimeMS;
    IntRoller frameTimes;

    std::function<void(unsigned long)> fun;

    XTaskTimer(unsigned long frameTime, const char *name, int historySize, std::function<void(unsigned long)> fun);
};


#endif //LED_FAN_XTASKTIMER_H
