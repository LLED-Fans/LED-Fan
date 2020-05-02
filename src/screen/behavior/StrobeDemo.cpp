//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "StrobeDemo.h"
#include <screen/Screen.h>

bool StrobeDemo::update(Screen *screen, unsigned long delay) {
    if (currentDelay > maxDelay)
        return false;

    if (timeUntilSwitch > delay)
        timeUntilSwitch -= delay;
    else {
        currentDelay *= stepUp;
        timeUntilSwitch = currentDelay;

        isWhite = !isWhite;
        fill_solid(screen->leds, screen->ledCount, isWhite ? CRGB::White : CRGB::Black);
    }

    return true;
}
