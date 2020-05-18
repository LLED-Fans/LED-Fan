//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "Ping.h"
#include <screen/Screen.h>

Ping::Ping(unsigned long timeLeft) : timeLeft(timeLeft) {}

bool Ping::update(Screen *screen, unsigned long delay) {
    if (timeLeft <= 0)
        return false;

    fill_solid(screen->leds, screen->ledCount, CRGB(
            (((timeLeft - 1) / blinkTime) % 2) == 0 ? 0 : 255,
        0,
        0
    ));

    timeLeft = timeLeft > delay
        ? timeLeft - delay
        : 0;

    return true;

}

