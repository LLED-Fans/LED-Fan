//
// Created by Lukas Tenbrink on 18.04.20.
//

#ifndef LED_FAN_MATH_H
#define LED_FAN_MATH_H

template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //LED_FAN_MATH_H
