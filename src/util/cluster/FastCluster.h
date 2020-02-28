//
// Created by Lukas Tenbrink on 28.02.20.
//

#ifndef LED_FAN_FASTCLUSTER_H
#define LED_FAN_FASTCLUSTER_H

#include <vector>

class FastCluster {
public:
    static std::vector<double> stepDiffs(std::vector<double> x);
    static double center(std::vector<double> x, double lenience);
};


#endif //LED_FAN_FASTCLUSTER_H
