//
// Created by Lukas Tenbrink on 25.04.20.
//

#ifndef LED_FAN_NATIVEBEHAVIOR_H
#define LED_FAN_NATIVEBEHAVIOR_H

#include <WString.h>

class Screen;

class NativeBehavior {
public:
    enum Status {
        dead, alive, purgatory
    };

    virtual String name() { return "Unknown Behavior"; };
    virtual Status update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_NATIVEBEHAVIOR_H
