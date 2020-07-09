//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_PERFECTSTROBE_H
#define LED_FAN_PERFECTSTROBE_H


#include <screen/Renderer.h>
#include "NativeBehavior.h"

class PerfectStrobe : public NativeBehavior {
public:
    PRGB color;
    bool isOn = false;
    float lastRotation = 0.0f;

    PerfectStrobe(const PRGB &color = PRGB::white);

    String name() override { return "Strobe"; };
    Status update(Screen *screen, unsigned long delay) override;
};


#endif //LED_FAN_PERFECTSTROBE_H
