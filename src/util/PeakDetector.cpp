//
// Created by Lukas Tenbrink on 20.01.20.
//

#include "PeakDetector.h"

bool PeakDetector::accept(int value) {
    if (this->lower == INT_MAX) {
        this->lower = value;
        this->upper = value;
        return true;
    }

    float d = value * decay;

    if (value < this->lower) {
        this->lower = value;
        this->upper = (int)((float)this->upper * this->keep + d);
    }
    else if (value > this->upper) {
        this->upper = value;
        this->lower = (int)((float)this->lower * this->keep + d);
    }
    else {
        this->upper = (int)((float)this->upper * this->keep + d);
        this->lower = (int)((float)this->lower * this->keep + d);
    }

    return true;
}
