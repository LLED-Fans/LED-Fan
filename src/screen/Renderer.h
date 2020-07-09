//
// Created by Lukas Tenbrink on 05.07.20.
//

#ifndef LED_FAN_RENDERER_H
#define LED_FAN_RENDERER_H


#include <cstdint>
#include <cstddef>

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

    inline PRGB& operator= (const PRGB& rhs) __attribute__((always_inline)) = default;

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

class Renderer {
public:
    size_t pixelCount;
    PRGB *rgb;

    explicit Renderer(size_t pixelCount, size_t overflowWall);

    virtual void render();

    virtual void setColorCorrection(PRGB correction);
    virtual PRGB getColorCorrection();

    virtual void setBrightness(float brightness);
    virtual float getBrightness();

    virtual void setLocalBrightness(float *brightness);
    virtual float *getLocalBrightness();

    // Color intensity response, for rescaled colors
    virtual void setResponse(float response);
    virtual float getResponse();

    // Maximum number of color components set to '1' (or some ratio thereof)
    virtual void setMaxLightness(float lightness);
    virtual float getMaxLightness();
protected:
    int _overflowWall;

    float _response;
    float _brightness;
    float *_localBrightness;
    PRGB _colorCorrection;

    // Array of 0-255^4 values used temporarily when render() is called
    uint32_t *_rgbOutput;
    // Remapping array from 0-255 values to 0-255^3 values
    uint32_t *_brightnessLUT;
    // Factor array for each local component. 0 to 255.
    uint32_t *_componentLUT;

    float _maxLightness;

    // Flushes the current output to be rendered
    virtual void _flush() {};

    // Flushes the lookup table
    void _flushLUT();
};


#endif //LED_FAN_RENDERER_H
