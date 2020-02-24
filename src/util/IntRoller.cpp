//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "IntRoller.h"

#include <algorithm>
#include <climits>

IntRoller::IntRoller(int c) : count(c), head(0) {
    data = new int[c];
    fill(0);
}

int& IntRoller::operator[](int index) {
    return data[(((head + index) % count + count) % count)];
}

void IntRoller::push(int value) {
    data[head] = value;
    head = (head + 1) % count;
}

void IntRoller::fill(int value) {
    for (int i = 0; i < count; i++)
        data[i] = value;
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

int IntRoller::countOccurrences(int v) {
    int c = 0;
    for (int i = 0; i < count; ++i) {
        if (data[i] == v)
            c++;
    }
    return c;
}
