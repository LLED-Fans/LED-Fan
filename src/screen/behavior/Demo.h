//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_DEMO_H
#define LED_FAN_DEMO_H


#include "RotatingBehavior.h"

class Demo : public RotatingBehavior {
public:
    float *speeds = nullptr;
    PRGB *colors = nullptr;
    float *sizes = nullptr;
    float *onRatios = nullptr;

    Demo();

    String name() override { return "Demo"; }
    virtual NativeBehavior::Status render(Screen *screen, unsigned long delay, float rotation);

    ~Demo();
private:
    void _populate(Screen *screen);
};


#endif //LED_FAN_DEMO_H
