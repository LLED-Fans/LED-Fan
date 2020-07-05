//
// Created by Lukas Tenbrink on 25.04.20.
//

#ifndef LED_FAN_NATIVEBEHAVIOR_H
#define LED_FAN_NATIVEBEHAVIOR_H

class Screen;

class NativeBehavior {
public:
    virtual bool update(Screen *screen, unsigned long delay);
};


#endif //LED_FAN_NATIVEBEHAVIOR_H
