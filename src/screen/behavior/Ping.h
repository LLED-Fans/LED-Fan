//
// Created by Lukas Tenbrink on 25.04.20.
//

#ifndef LED_FAN_PING_H
#define LED_FAN_PING_H


#include "NativeBehavior.h"

class Ping : public NativeBehavior {
public:
    unsigned long timeLeft;
    unsigned long blinkTime = 500 * 1000;

    Ping(unsigned long timeLeft);

    virtual bool update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_PING_H
