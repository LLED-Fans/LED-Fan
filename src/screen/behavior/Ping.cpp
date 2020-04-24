//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "Ping.h"

Ping::Ping(int timeLeft) : timeLeft(timeLeft) {}

bool Ping::update(CRGB *leds, int ledCount, unsigned long delay) {
    if (timeLeft <= 0)
        return false;

    fill_solid(leds, ledCount, CRGB(
        (((timeLeft - 1) / 500) % 2) == 0 ? 0 : 255,
        0,
        0
    ));

    timeLeft = timeLeft > delay
        ? timeLeft - delay
        : 0;

    return true;

}

