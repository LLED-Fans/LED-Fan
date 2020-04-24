//
// Created by Lukas Tenbrink on 25.04.20.
//

#ifndef LED_FAN_NATIVEBEHAVIOR_H
#define LED_FAN_NATIVEBEHAVIOR_H

#include <FastLED.h>

class NativeBehavior {
public:
    virtual bool update(CRGB *leds, int ledCount, unsigned long delay);
};


#endif //LED_FAN_NATIVEBEHAVIOR_H
