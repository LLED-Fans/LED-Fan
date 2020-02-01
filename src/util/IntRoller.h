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

    IntRoller(int c): count(c) {
        this->data = new int[c];
    }

    int operator[](int index) {
        return data[index];
    }

    void add(int value) {
        this->head = (this->head + 1) % this->count;
        this->data[head] = value;
    }

    int last() {
        return this->data[head];
    }

    int min() {
        return *std::min_element(this->data, this->data + this->count);
    }

    int max() {
        return *std::max_element(this->data, this->data + this->count);
    }

    int sum() {
        return std::accumulate(this->data, this->data + this->count, 0);
    }
private:
    int head;
};

#endif //LED_FAN_INTROLLER_H
