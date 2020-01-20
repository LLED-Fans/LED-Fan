//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_DATAROLLER_H
#define LED_FAN_DATAROLLER_H


#include <algorithm>

class DataRoller {
public:
    const int count;
    int *data;

    DataRoller(int c): count(c) {
        this->data = new int[c];
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
private:
    int head;
};

#endif //LED_FAN_DATAROLLER_H
