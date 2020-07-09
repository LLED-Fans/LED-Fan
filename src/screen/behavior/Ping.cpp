//
// Created by Lukas Tenbrink on 25.04.20.
//

#include "Ping.h"
#include <screen/Screen.h>

Ping::Ping(unsigned long timeLeft) : timeLeft(timeLeft) {}

NativeBehavior::Status Ping::update(Screen *screen, unsigned long delay) {
    if (timeLeft <= 0)
        return dead;

    Renderer *renderer = screen->renderer;
    PRGB((((timeLeft - 1) / blinkTime) % 2) == 0 ? PRGB::black : PRGB::red)
        .fill(renderer->rgb, renderer->pixelCount);

    timeLeft = timeLeft > delay
        ? timeLeft - delay
        : 0;

    return alive;
}

