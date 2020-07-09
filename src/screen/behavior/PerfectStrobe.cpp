//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <esp32-hal.h>
#include "PerfectStrobe.h"

#include "screen/Screen.h"

PerfectStrobe::PerfectStrobe(const PRGB &color) : color(color) {}

NativeBehavior::Status PerfectStrobe::update(Screen *screen, unsigned long delay) {
    float rotation = screen->rotationSensor->estimatedRotation(micros());
    if (rotation < lastRotation)
        isOn = !isOn;
    lastRotation = rotation;

    Renderer *renderer = screen->renderer;
    PRGB(isOn ? color : PRGB::black)
        .fill(renderer->rgb, renderer->pixelCount);

    return purgatory;
}
