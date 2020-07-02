//
// Created by Lukas Tenbrink on 20.04.20.
//

#include <cmath>
#include "LUT.h"

LUT::Table *LUT::create_LUT(int count, float min, float max, std::function<float(float)> fun) {
    auto table = new Table();
    table->values = new float[count];
    table->a = float(count - 1) / (max - min);
    table->b = -min;
    table->count = count;

    for (int i = 0; i < count; ++i) {
        table->values[i] = fun(min + i / table->a);
    }

    return table;
}

float LUT::Table::lookup(float key) {
    return values[lroundf((key + b) * a)];
}

float LUT::Table::interpolate(float key) {
    float lookup = (key + b) * a;
    int bottom = int(lookup);
    float part = lookup - float(bottom);
    return values[bottom] * part + values[bottom + 1] * (1.0f - part);
}

LUT::Table *LUT::sin = nullptr;
LUT::Cos *LUT::cos = nullptr;
void LUT::initSin(int count) {
    LUT::sin = create_LUT(count, 0, M_TWOPI, [](float v){
        return std::sin(v);
    });
    cos = new Cos();
}

float LUT::Cos::lookup(float key) {
    return LUT::sin->lookup(fmodf(key + M_PI_2, M_TWOPI));
}

float LUT::Cos::interpolate(float key) {
    return LUT::sin->interpolate(fmodf(key + M_PI_2, M_TWOPI));
}
