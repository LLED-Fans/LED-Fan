//
// Created by Lukas Tenbrink on 11.07.20.
//

#include <util/LUT.h>
#include "FancyPing.h"
#include "screen/Screen.h"

NativeBehavior::Status FancyPing::update(Screen *screen, unsigned long delay) {
    if (timeLeft < delay)
        return dead;

    timeLeft -= delay;
    for (int i = 0; i < screen->getPixelCount(); ++i) {
        auto pixel = screen->pixels[i];
        
        float activation = 0;
        float activationTime = float(timeLeft) * 4 / (1000 * 1000) - pixel->radius;

        if (activationTime < M_TWOPI && activationTime >= 0.0f)
            activation = std::abs(LUT::sin->interpolate(activationTime));

        *pixel->color = PHSV(i + timeLeft / 1000 / 20, 255, 100 / pixel->radius * activation);
    }

    return alive;
}
