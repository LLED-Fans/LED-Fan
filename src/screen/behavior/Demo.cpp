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

    int milliseconds = millis();
    int pixels = screen->getPixelCount();

    // MovingShift
    // always positive so that we can use % later
    float shift = fmodf(-float(milliseconds) / (20 * 1000.0f) * pixels, pixels) + pixels;
    int shiftLow = int(shift);
    float prevRatio = shift - shiftLow;
    float toRatio = 1.0f - prevRatio;

    float bladeRotations[screen->bladeCount];
    for (int j = 0; j < screen->bladeCount; ++j)
        bladeRotations[j] = std::fmod(rotation + screen->blades[j]->rotationOffset, 1.0f);

    // Can't interpolate between prev / next pixels because neighbors are always on the other sides...
    for (int i = 0; i < pixels; ++i) {
        Blade::Pixel *pixel = screen->pixels[i];
        auto bladeRotation = bladeRotations[pixel->bladeIndex];

        PRGB rColors[2];
        for (int s = 0; s < 2; ++s) {
            int pIdx = (i + shiftLow + s) % pixels;

            float position = fmodf(milliseconds * speeds[pIdx] + bladeRotation, sizes[pIdx]);
            if (position < 0) // fmodf can return negative values
                position += sizes[pIdx];

            rColors[s] = position < onRatios[pIdx] ? colors[pIdx] : PRGB::black;
        }

        *pixel->color = PRGB(
            rColors[1].r * prevRatio + rColors[0].r * toRatio,
            rColors[1].g * prevRatio + rColors[0].g * toRatio,
            rColors[1].b * prevRatio + rColors[0].b * toRatio
        );
    }

    return purgatory;
}

void Demo::_populate(Screen *screen) {
    int pixels = screen->getPixelCount();
    sizes = new float[pixels];
    colors = new PRGB[pixels + 1];
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
    // For interpolation easyness
    colors[pixels] = colors[0];
}

Demo::~Demo() {
    delete(sizes);
    delete(colors);
    delete(speeds);
    delete(onRatios);
}
