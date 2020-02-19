//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_INTROLLER_H
#define LED_FAN_INTROLLER_H


#include <algorithm>
#include <cstring>
#include <climits>
#include "RollerIterator.h"

class IntRoller {
public:
    const unsigned int count;
    int *data;

    IntRoller(int c): count(c), head(c - 1) {
        data = new int[c];
        fill(0);
    }

    int operator[](int index) {
        return data[index];
    }

    void append(int value) {
        head = (head + 1) % count;
        data[head] = value;
    }

    void fill(int value) {
        for (int i = 0; i < count; i++)
            data[i] = value;
    }

    float scalesolidMean(float threshold, int *sCount) {
        int mean = sum() / count;

        int solidSum = 0;
        int solidCount = 0;

        if (mean != 0) {
            for (int i = 0; i < count; ++i) {
                if (abs((data[i] - mean) / mean) < threshold) {
                    solidSum += data[i];
                    solidCount ++;
                }
            }
        }

        if (sCount) *sCount = solidCount;

        return solidCount > 0 ? (float) solidSum / (float) solidCount : INT_MAX;
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

    float mean() {
        return (float) sum() / (float) count;
    }

    RollerIterator<int> begin() {return {data, head, count};}
    RollerIterator<int> end() {return {};}

private:
    unsigned int head = 0;
};

#endif //LED_FAN_INTROLLER_H
