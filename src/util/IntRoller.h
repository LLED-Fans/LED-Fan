//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_INTROLLER_H
#define LED_FAN_INTROLLER_H


#include "RollerIterator.h"

class IntRoller {
public:
    const unsigned int count;
    int *data;

    IntRoller(int c);

    int operator[](int index);

    void append(int value);

    void fill(int value);

    float scalesolidMean(float threshold, int *sCount);

    int last() {
        return data[head];
    }

    int min();
    int max();

    int sum();
    float mean();

    RollerIterator<int> begin() { return {data, head, count}; }

    RollerIterator<int> end() { return {}; }

private:
    unsigned int head = 0;
};

#endif //LED_FAN_INTROLLER_H
