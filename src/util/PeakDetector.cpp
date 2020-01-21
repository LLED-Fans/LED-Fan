//
// Created by Lukas Tenbrink on 20.01.20.
//

#include <HardwareSerial.h>
#include "PeakDetector.h"

bool PeakDetector::accept(int value) {
    if (this->lower == INT_MAX) {
        this->lower = value;
        this->upper = value;
        return true;
    }

    if (value < this->lower) {
        this->lower = value;
        this->upper = (int)((float)this->upper * this->keep + value * decay);
    }
    else if (value > this->upper) {
        this->upper = value;
        this->lower = (int)((float)this->lower * this->keep + value * decay);
    }
    else {
        this->upper = (int)((float)this->upper * this->keep + value * decay);
        this->lower = (int)((float)this->lower * this->keep + value * decay);
    }

    return true;
}
