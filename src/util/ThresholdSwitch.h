//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_THRESHOLDSWITCH_H
#define LED_FAN_THRESHOLDSWITCH_H


class ThresholdSwitch {
public:
    int lower, upper;
    bool isOn = false;

    ThresholdSwitch(int lower, int upper): lower(lower), upper(upper) {
    }

    bool accept(int value);
};


#endif //LED_FAN_THRESHOLDSWITCH_H
