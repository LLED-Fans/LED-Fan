//
// Created by Lukas Tenbrink on 02.05.20.
//

#ifndef LED_FAN_DOTTED_H
#define LED_FAN_DOTTED_H


#include "NativeBehavior.h"

class Dotted : public NativeBehavior {
public:
    bool isWhite = false;

    Dotted();

    String name() override { return "Dotted"; }
    virtual NativeBehavior::Status update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_DOTTED_H
