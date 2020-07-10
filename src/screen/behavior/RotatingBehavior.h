//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_ROTATINGBEHAVIOR_H
#define LED_FAN_ROTATINGBEHAVIOR_H


#include "NativeBehavior.h"

class RotatingBehavior : public NativeBehavior {
public:
    virtual NativeBehavior::Status update(Screen *screen, unsigned long delay);
    virtual NativeBehavior::Status render(Screen *screen, unsigned long delay, float rotation) {
        return dead;
    };
private:
};


#endif //LED_FAN_ROTATINGBEHAVIOR_H
