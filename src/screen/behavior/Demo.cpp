//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <esp32-hal.h>
#include <util/Random.h>
#include <util/LUT.h>
#include <util/Logger.h>
#include <cmath>
#include "Demo.h"

#include "screen/Screen.h"

Demo::Demo() {
}

NativeBehavior::Status Demo::render(Screen *screen, unsigned long delay, float rotation) {
    if (!sizes)
        _populate(screen);

    auto milliseconds = millis();

    for (int b = 0; b < screen->bladeCount; ++b) {
        auto blade = screen->blades[b];
        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);

        for (int p = blade->pixelCount - 1; p >= 0; --p) {
            Blade::Pixel &pixel = blade->pixels[p];
            int pIdx = pixel.pixelIndex;

            float position = fmodf(milliseconds * speeds[pIdx] + bladeRotation, sizes[pIdx]);

            if (position < 0) // fmodf can return negative values
                position += sizes[pIdx];

            *pixel.color = position < onRatios[pIdx] ? colors[pIdx] : PRGB::black;
        }
    }

    return purgatory;
}

void Demo::_populate(Screen *screen) {
    int pixels = screen->getPixelCount();
    sizes = new float[pixels];
    colors = new PRGB[pixels];
    speeds = new float[pixels];
    onRatios = new float[pixels];

    uint8_t lastHue = 0;
    for (int i = 0; i < pixels; ++i) {
        // Must divide 1.0 into equal pieces
        sizes[i] = 1.0f / random(2, 12);
        PHSV color = PHSV(random(0, 256), random(0, 256), random(0, 256));
        if (lastHue != 0)
            color.h = (color.h >> 1) + lastHue - uint8_t(64);
        if (color.v < 100)
            color.v = 0;
        colors[i] = color;
        lastHue = color.h;
        speeds[i] = randomf(0.1f, 0.3f) / 1000.0f * (random(2) ? 1 : -1);
        onRatios[i] = randomf(0.05f, 0.3f) * sizes[i];
    }
}

Demo::~Demo() {
    delete(sizes);
    delete(colors);
    delete(speeds);
    delete(onRatios);
}
