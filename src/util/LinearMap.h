//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_LINEARMAP_H
#define LED_FAN_LINEARMAP_H

#include <vector>

template<typename KEY, typename VALUE>
class LinearMap {
public:
    std::vector<KEY> keys;
    std::vector<VALUE> values;

    int size() {
        return keys.size();
    }

    void clear() {
        keys.clear();
        values.clear();
    }

    void put(KEY key, VALUE value) {
        auto *existing = (*this)[key];
        if (existing != nullptr)
            *existing = value;
        else {
            keys.push_back(key);
            values.push_back(value);
        }
    }

    VALUE* operator[] (KEY key) {
        for (int i = 0; i < keys.size(); ++i) {
            if (keys[i] == key)
                return &values[i];
        }

        return nullptr;
    }
};


#endif //LED_FAN_LINEARMAP_H
