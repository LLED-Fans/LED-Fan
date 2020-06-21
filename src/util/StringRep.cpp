//
// Created by Lukas Tenbrink on 22.06.20.
//

#include "StringRep.h"

float StringRep::toFloat(String s, float def) {
    float f = s.toFloat();

    // toFloat returns 0 as default, lol
    // try to see if it's really 0 by hardcoding...
    if (f == 0 && s != "0" && s != "0.0")
        return def;

    return f;
}
