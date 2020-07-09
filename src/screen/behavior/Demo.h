//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_DEMO_H
#define LED_FAN_DEMO_H


#include "NativeBehavior.h"

class Demo : public NativeBehavior {
public:
    Demo();

    virtual NativeBehavior::Status update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_DEMO_H
