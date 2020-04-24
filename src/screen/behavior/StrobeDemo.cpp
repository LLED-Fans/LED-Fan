//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "StrobeDemo.h"

bool StrobeDemo::update(CRGB *leds, int ledCount, unsigned long delay) {
    if (currentDelay > maxDelay)
        return false;

    if (timeUntilSwitch > delay)
        timeUntilSwitch -= delay;
    else {
        currentDelay *= stepUp;
        timeUntilSwitch = currentDelay;

        isWhite = !isWhite;
        fill_solid(leds, ledCount, isWhite ? CRGB::White : CRGB::Black);
    }

    return true;
}
