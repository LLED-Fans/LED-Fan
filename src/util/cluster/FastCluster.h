//
// Created by Lukas Tenbrink on 28.02.20.
//

#ifndef LED_FAN_FASTCLUSTER_H
#define LED_FAN_FASTCLUSTER_H

#include <vector>
#include <cmath>
#include <cstring>

class FastCluster {
public:
    template<typename T>
    static T center(std::vector<T> x, T lenience) {
        int n = x.size();

        int counts[n];
        memset(counts, 0, n * sizeof(int));

        // n log n
        int maxIndex = 0;

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (std::abs(x[i] - x[j]) < lenience) {
                    counts[i]++;
                    counts[j]++;
                }
            }

            // We can do this here because counts are complete
            // When j loop is done - we don't go backwards
            if (counts[i] > counts[maxIndex])
                maxIndex = i;
        }

        return x[maxIndex];
    }

    template<typename T>
    static std::vector<T> stepDiffs(std::vector<T> x) {
        unsigned int n = x.size() - 1;
        auto r = std::vector<T>(n);

        for (int i = 0; i < n; ++i) {
            r[i] = x[i + 1] - x[i];
        }

        return r;
    }
};


#endif //LED_FAN_FASTCLUSTER_H
