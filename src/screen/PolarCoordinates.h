//
// Created by Lukas Tenbrink on 01.02.20.
//

#ifndef LED_FAN_POLARCOORDINATES_H
#define LED_FAN_POLARCOORDINATES_H


class PolarCoordinates {
public:
    static void asCartesian(float theta, float r, float *x, float *y);
};


#endif //LED_FAN_POLARCOORDINATES_H
