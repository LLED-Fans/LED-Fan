//
// Created by Lukas Tenbrink on 11.07.20.
//

#ifndef LED_FAN_CARTESIANDEMO_H
#define LED_FAN_CARTESIANDEMO_H


#include "RotatingBehavior.h"

class CartesianDemo : public RotatingBehavior {
public:
    String name() override { return "Cartesian Demo"; }
    virtual NativeBehavior::Status render(Screen *screen, unsigned long delay, float rotation);
};


#endif //LED_FAN_CARTESIANDEMO_H
