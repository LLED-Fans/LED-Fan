//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <esp32-hal.h>
#include "Demo.h"

#include "screen/Screen.h"

NativeBehavior::Status Demo::update(Screen *screen, unsigned long delay) {
    if (!screen->rotationSensor->isReliable())
        return purgatory;

    float rotation = screen->rotationSensor->estimatedRotation(micros());
    auto milliseconds = millis();

    for (int b = 0; b < screen->bladeCount; ++b) {
        auto blade = screen->blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        auto hueShift = milliseconds * 255 / 1000 / 10  + (unsigned long)(bladeRotation * 255);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];

            if (bladeRotation > 0.75f) {
                *pixel.color = PRGB::white;
            }
            else {
                // TODO Fix
                *pixel.color = PRGB::black;
//                fill_rainbow(
//                    pixel.color, 1,
//                    pixel.radius * 10 + hueShift,
//                    0
//                );
            }
        }
    }

    return purgatory;
}
