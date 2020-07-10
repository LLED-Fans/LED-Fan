//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <screen/Renderer.h>
#include <esp32-hal.h>
#include "screen/Screen.h"
#include "RotatingBehavior.h"

NativeBehavior::Status RotatingBehavior::update(Screen *screen, unsigned long delay) {
    if (!screen->rotationSensor->isReliable()) {
        Renderer *renderer = screen->renderer;
        PRGB(PRGB::black)
            .fill(renderer->rgb, renderer->pixelCount);

        return purgatory;
    }

    return render(screen, delay, screen->rotationSensor->estimatedRotation(micros()));
}
