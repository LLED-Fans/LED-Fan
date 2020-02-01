//
// Created by Lukas Tenbrink on 01.02.20.
//

#ifndef LED_FAN_CONCENTRICCOORDINATES_H
#define LED_FAN_CONCENTRICCOORDINATES_H


#include <util/IntRoller.h>

class ConcentricCoordinates {
public:
    static float* sampledCartesian(IntRoller *resolution);

    static IntRoller *resolution(int ringCount);
    static int ringResolution(int ring);
};


#endif //LED_FAN_CONCENTRICCOORDINATES_H
