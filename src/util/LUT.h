//
// Created by Lukas Tenbrink on 20.04.20.
//

#ifndef LED_FAN_LUT_H
#define LED_FAN_LUT_H


#include <functional>

class LUT {
public:
    class Table {
    public:
        int count;
        float *values;

        float a, b;

        float lookup(float key);
        float interpolate(float key);
    };

    class Cos {
    public:
        float lookup(float key);
        float interpolate(float key);
    };

    static Table *create_LUT(int count, float min, float max, std::function<float(float)> fun);

    static Table *sin;
    static Cos *cos ;
    static void initSin(int count);
};


#endif //LED_FAN_LUT_H
