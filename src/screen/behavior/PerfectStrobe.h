//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_PERFECTSTROBE_H
#define LED_FAN_PERFECTSTROBE_H


#include "NativeBehavior.h"

class PerfectStrobe : public NativeBehavior {
public:
    PRGB color;
    bool isOn = false;
    unsigned long timeUntilSwitch = 100 * 1000;
    float lastRotation = 0.0f;

    PerfectStrobe(const PRGB &color = PRGB::white);

    String name() override { return "Strobe"; };
    Status update(Screen *screen, unsigned long delay) override;
};


#endif //LED_FAN_PERFECTSTROBE_H
