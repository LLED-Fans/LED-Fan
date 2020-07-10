//
// Created by Lukas Tenbrink on 10.07.20.
//

#include <cmath>
#include "Pixels.h"

void PHSV::toRGB(PRGB *rgb) const {
    double H = h / 255.0 * 6.0, S = s / 255.0, V = v / 255.0;

    double fract = H - floor(H);

    double P = V * (1. - S) * 255.0;
    double Q = V * (1. - S * fract) * 255.0;
    double T = V * (1. - S * (1. - fract)) * 255.0;

    if (0. <= H && H < 1.)
        *rgb = PRGB(V, T, P);
    else if (1. <= H && H < 2.)
        *rgb = PRGB(Q, V, P);
    else if (2. <= H && H < 3.)
        *rgb = PRGB(P, V, T);
    else if (3. <= H && H < 4.)
        *rgb = PRGB(P, Q, V);
    else if (4. <= H && H < 5.)
        *rgb = PRGB(T, P, V);
    else if (5. <= H && H < 6.)
        *rgb = PRGB(V, P, P);
    else
        *rgb = PRGB::black;
}
