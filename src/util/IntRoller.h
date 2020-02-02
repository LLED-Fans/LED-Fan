//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_INTROLLER_H
#define LED_FAN_INTROLLER_H


#include <algorithm>

class IntRoller {
public:
    const int count;
    int *data;

    IntRoller(int c): count(c), head(c - 1) {
        data = new int[c];
    }

    int operator[](int index) {
        return data[index];
    }

    void add(int value) {
        head = (head + 1) % count;
        data[head] = value;
    }

    int last() {
        return data[head];
    }

    int min() {
        return *std::min_element(data, data + count);
    }

    int max() {
        return *std::max_element(data, data + count);
    }

    int sum() {
        return std::accumulate(data, data + count, 0);
    }
private:
    int head;
};

#endif //LED_FAN_INTROLLER_H
