//
// Created by Lukas Tenbrink on 25.04.20.
//

#ifndef LED_FAN_STROBEDEMO_H
#define LED_FAN_STROBEDEMO_H


#include "NativeBehavior.h"

class StrobeDemo : public NativeBehavior {
public:
    double currentDelay = 1;
    double stepUp = 1.01;
    double maxDelay = 50;

    bool isWhite = false;
    unsigned long timeUntilSwitch;

    virtual bool update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_STROBEDEMO_H
