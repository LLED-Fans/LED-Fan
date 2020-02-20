//
// Created by Lukas Tenbrink on 20.02.20.
//

#ifndef LED_FAN_POLYNOMIAL_H
#define LED_FAN_POLYNOMIAL_H


#include "IntRoller.h"
#include <vector>

class Polynomial {
public:
    static void linearRegression(std::vector<double> x, std::vector<double> y, double *a, double *b);
};


#endif //LED_FAN_POLYNOMIAL_H
