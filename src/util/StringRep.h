//
// Created by Lukas Tenbrink on 22.06.20.
//

#ifndef LED_FAN_STRINGREP_H
#define LED_FAN_STRINGREP_H


#include <WString.h>

class StringRep {
public:
    static float toFloat(String s, float def);
};


#endif //LED_FAN_STRINGREP_H
