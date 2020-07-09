//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "StrobeDemo.h"
#include <screen/Screen.h>

NativeBehavior::Status StrobeDemo::update(Screen *screen, unsigned long delay) {
    if (currentDelay > maxDelay)
        return dead;

    if (timeUntilSwitch > delay)
        timeUntilSwitch -= delay;
    else {
        currentDelay *= stepUp;
        timeUntilSwitch = currentDelay;

        isWhite = !isWhite;

        Renderer *renderer = screen->renderer;
        PRGB(isWhite ? PRGB::white : PRGB::black)
            .fill(renderer->rgb, renderer->pixelCount);
    }

    return alive;
}
