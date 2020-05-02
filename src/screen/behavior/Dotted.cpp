//
// Created by Lukas Tenbrink on 02.05.20.
//

#include "Dotted.h"
#include <screen/Screen.h>

Dotted::Dotted() {}

bool Dotted::update(Screen *screen, unsigned long delay) {
    if (screen->rotationSensor->didUpdate)
        isWhite = false; // Keep it as synced as we can

    fill_solid(screen->leds, screen->ledCount, isWhite ? CRGB::White : CRGB::Black);
    isWhite = !isWhite;

    return true;

}

