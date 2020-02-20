//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "IntRoller.h"

#include <algorithm>
#include <climits>

IntRoller::IntRoller(int c) : count(c), head(c - 1) {
    data = new int[c];
    fill(0);
}

int IntRoller::operator[](int index) {
    return data[index];
}

void IntRoller::append(int value) {
    head = (head + 1) % count;
    data[head] = value;
}

void IntRoller::fill(int value) {
    for (int i = 0; i < count; i++)
        data[i] = value;
}

float IntRoller::scalesolidMean(float threshold, int *sCount) {
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

int IntRoller::min() {
    return *std::min_element(data, data + count);
}

int IntRoller::max() {
    return *std::max_element(data, data + count);
}

int IntRoller::sum() {
    return std::accumulate(data, data + count, 0);
}

float IntRoller::mean() {
    return (float) sum() / (float) count;
}
