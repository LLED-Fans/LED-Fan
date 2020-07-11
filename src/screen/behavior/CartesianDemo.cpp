//
// Created by Lukas Tenbrink on 11.07.20.
//

#include <esp32-hal.h>
#include <screen/PolarCoordinates.h>
#include "CartesianDemo.h"

#include "screen/Screen.h"

NativeBehavior::Status CartesianDemo::render(Screen *screen, unsigned long delay, float rotation) {
    for (int b = 0; b < screen->bladeCount; ++b) {
        auto blade = screen->blades[b];

        float vectorX, vectorY;
        PolarCoordinates::asCartesian(
                std::fmod((rotation + blade->rotationOffset), 1.0f) * float(M_TWOPI),
                screen->cartesianCenter,
                &vectorX, &vectorY
        );

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];
            int x = std::lroundf(screen->cartesianCenter + vectorX * pixel.radius);
            int y = std::lroundf(screen->cartesianCenter + vectorY * pixel.radius);

            *pixel.color = PRGB(
                ((x + y) % 3) == 0 ? 255 : 0,
                ((x + y) % 3) == 1 ? 255 : 0,
                ((x + y) % 2) == 0 ? 255 : 0
            );
        }
    }

    return purgatory;
}
