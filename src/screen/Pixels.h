//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_PIXELS_H
#define LED_FAN_PIXELS_H

#include <cstdint>

struct PRGB;

struct PHSV {
    union {
        struct {
            uint8_t h;
            uint8_t s;
            uint8_t v;
        };
        uint8_t components[3];
    };

    inline PHSV() __attribute__((always_inline)) {}

    inline PHSV( uint8_t ih, uint8_t is, uint8_t iv) __attribute__((always_inline))
    : h(ih), s(is), v(iv) {}

    void toRGB(PRGB *rgb) const;
};

struct PRGB {
    union {
        struct {
            uint8_t r, g, b;
        };
        uint8_t components[3];
    };

    inline PRGB() __attribute__((always_inline))
    {
    }

    inline PRGB( uint8_t ir, uint8_t ig, uint8_t ib)  __attribute__((always_inline))
            : r(ir), g(ig), b(ib)
    {
    }

    inline PRGB( uint32_t colorcode)  __attribute__((always_inline))
            : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }

    inline PRGB(const PRGB& rhs) __attribute__((always_inline)) = default;

    inline PRGB(const PHSV& rhs) __attribute__((always_inline))
    {
        rhs.toRGB(this);
    }

    inline PRGB& operator= (const PRGB& rhs) __attribute__((always_inline)) = default;

    inline PRGB& operator= (const PHSV& rhs) __attribute__((always_inline))
    {
        rhs.toRGB(this);
        return *this;
    }

    inline PRGB& operator= (const uint32_t colorcode) __attribute__((always_inline))
    {
        *this = PRGB(colorcode);
        return *this;
    }

    void fill(PRGB *array, int count) {
        for (int i = 0; i < count; ++i) {
            array[i] = *this;
        }
    }

    typedef enum {
        black = 0x000000,
        white = 0xffffff,
        red = 0xff0000,
        green = 0x00ff00,
        blue = 0x0000ff
    } HTMLColorCode;
};

#endif //LED_FAN_PIXELS_H
