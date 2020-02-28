//
// Created by Lukas Tenbrink on 28.02.20.
//

#include <cmath>
#include "FastCluster.h"

double FastCluster::center(std::vector<double> x, double lenience) {
    int n = x.size();

    int counts[n];

    // n log n
    int maxIndex = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (std::abs(x[i] - x[j]) < lenience) {
                counts[i] ++;
                counts[j] ++;
            }
        }

        // We can do this here because counts are complete
        // When j loop is done - we don't go backwards
        if (counts[i] > counts[maxIndex])
            maxIndex = i;
    }

    return x[maxIndex];
}

std::vector<double> FastCluster::stepDiffs(std::vector<double> x) {
    unsigned int n = x.size() - 1;
    auto r = std::vector<double>{n};

    for (int i = 0; i < n; ++i) {
        r[n] = x[n + 1] - x[n];
    }

    return r;
}
