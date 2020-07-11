//
// Created by Lukas Tenbrink on 11.07.20.
//

#ifndef LED_FAN_FANCYPING_H
#define LED_FAN_FANCYPING_H


#include <WString.h>
#include <math.h>
#include "NativeBehavior.h"

class FancyPing : public NativeBehavior {
public:
    unsigned long timeLeft = M_TWOPI * (1000.0 * 1000 / 4 + 500 * 1000);

    String name() override { return "Fancy Ping"; }
    NativeBehavior::Status update(Screen *screen, unsigned long delay) override;
};


#endif //LED_FAN_FANCYPING_H
