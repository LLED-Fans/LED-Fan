//
// Created by Lukas Tenbrink on 22.06.20.
//

#include "StringRep.h"

float StringRep::toFloat(String s, float def) {
    float v = s.toFloat();

    // toFloat returns 0 as default, lol
    // try to see if it's really 0 by hardcoding...
    if (v == 0 && s != "0" && s != "0.0")
        return def;

    return v;
}

int StringRep::toInt(String s, int def) {
    int v = s.toInt();

    if (v == 0 && s != "0")
        return def;

    return v;
}
