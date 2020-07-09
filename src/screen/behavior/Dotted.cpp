//
// Created by Lukas Tenbrink on 02.05.20.
//

#include "Dotted.h"
#include <screen/Screen.h>

Dotted::Dotted() {}

NativeBehavior::Status Dotted::update(Screen *screen, unsigned long delay) {
    if (screen->rotationSensor->didUpdate)
        isWhite = false; // Keep it as synced as we can

    auto renderer = screen->renderer;
    for (int i = 0; i < renderer->pixelCount; ++i) {
        renderer->rgb[i] = isWhite ? PRGB::white : PRGB::black;
    }
    isWhite = !isWhite;

    return purgatory;

}

