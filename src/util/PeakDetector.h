
//
// Created by Lukas Tenbrink on 20.01.20.
//

#ifndef LED_FAN_PEAKDETECTOR_H
#define LED_FAN_PEAKDETECTOR_H


class PeakDetector {
public:
    const float decay;
    const float keep;

    int lower;
    int upper;

    PeakDetector(float decay): decay(decay), keep(1.0f - decay) {
    }

    bool accept(int value);
};

#endif //LED_FAN_PEAKDETECTOR_H
