//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SENSORSWITCH_H
#define LED_FAN_SENSORSWITCH_H


#include <util/PeakDetector.h>
#include <util/ThresholdSwitch.h>

class SensorSwitch {
public:
    int pin;
    bool isReliable = false;

    PeakDetector *peaks;
    ThresholdSwitch *thresholdSwitch;

    SensorSwitch(int pin);

    bool test();

    bool isOn() {
        return thresholdSwitch->isOn;
    }
};


#endif //LED_FAN_SENSORSWITCH_H
