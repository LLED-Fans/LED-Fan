//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_SENSORSWITCH_H
#define LED_FAN_SENSORSWITCH_H


#include <cstdint>
#include <util/PeakDetector.h>
#include <util/ThresholdSwitch.h>

class SensorSwitch {
public:
    int pin;
    bool isReliable = false;

    PeakDetector *peaks;
    ThresholdSwitch *thresholdSwitch;

    SensorSwitch(int pin, PeakDetector *peaks);

    bool test();
    uint16_t rawValue();

    bool isOn() {
        return thresholdSwitch->isOn;
    }
};


#endif //LED_FAN_SENSORSWITCH_H
