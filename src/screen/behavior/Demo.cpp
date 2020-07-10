//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <esp32-hal.h>
#include <util/Random.h>
#include <util/LUT.h>
#include <util/Logger.h>
#include "Demo.h"

#include "screen/Screen.h"

Demo::Demo() {
}

NativeBehavior::Status Demo::update(Screen *screen, unsigned long delay) {
    SerialLog.print(esp_random());
    SerialLog.print(esp_random());
    SerialLog.print(esp_random());
    SerialLog.print(esp_random());

//    if (!sizes)
//        _populate(screen);
//
//    if (!screen->rotationSensor->isReliable())
//        return purgatory;
//
//    float rotation = screen->rotationSensor->estimatedRotation(micros());
//    auto milliseconds = millis();
//    auto sinBase = milliseconds / 1000.0f;
//
//    for (int b = 0; b < screen->bladeCount; ++b) {
//        auto blade = screen->blades[b];
//        auto bladeRotation = std::fmod(rotation + blade->rotationOffset, 1.0f);
//
//        for (int p = blade->pixelCount - 1; p >= 0; --p) {
//            Blade::Pixel &pixel = blade->pixels[p];
//            int pIdx = pixel.pixelIndex;
//
//            float pixelShift = LUT::sin->interpolate(sinBase + pixel.radius * 2.0f);
//
//            float position = fmodf(milliseconds * speeds[pIdx] + pixelShift + bladeRotation, sizes[pIdx]);
//            *pixel.color = position < onRatios[pIdx] ? colors[pIdx] : PRGB::black;
//        }
//    }

    return purgatory;
}

void Demo::_populate(Screen *screen) {
//    int pixels = screen->getPixelCount();
//    sizes = new float[pixels];
//    colors = new PRGB[pixels];
//    speeds = new float[pixels];
//    onRatios = new float[pixels];
//
//    for (int i = 0; i < pixels; ++i) {
//        sizes[i] = randomf(0.1f, 0.4f);
//        colors[i] = PHSV(random(0, 256), random(0, 256), random(0, 256));
//        speeds[i] = randomf(0.1f, 0.5f) / 1000.0f;
//        onRatios[i] = randomf(0.1f, 0.4f);
//    }
}

Demo::~Demo() {
    delete(sizes);
    delete(colors);
    delete(speeds);
    delete(onRatios);
}
