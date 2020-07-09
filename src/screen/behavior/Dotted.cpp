//
// Created by Lukas Tenbrink on 02.05.20.
//

#include "Dotted.h"
#include <screen/Screen.h>

Dotted::Dotted() {}

NativeBehavior::Status Dotted::update(Screen *screen, unsigned long delay) {
    if (screen->rotationSensor->didUpdate)
        isWhite = false; // Keep it as synced as we can

    Renderer *renderer = screen->renderer;
    PRGB(isWhite ? PRGB::white : PRGB::black)
        .fill(renderer->rgb, renderer->pixelCount);

    return purgatory;

}

