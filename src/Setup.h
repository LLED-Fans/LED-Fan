// ==================================================================
// ===================== Constants  =================================
// ==================================================================

#define ROTATION_SENSOR_TYPE_HALL_SYNC 1
#define ROTATION_SENSOR_TYPE_HALL_XTASK 2

// Good if measurements are accurate, but speed may vary
#define ROTATION_EXTRAPOLATION_STEP 1
// Good if speed is relatively constant, but measurements vary
#define ROTATION_EXTRAPOLATION_REGRESSION 2

// ==================================================================
// ======================  Setup  ===================================
// ==================================================================

#ifndef LED_FAN_SETUP_H
#define LED_FAN_SETUP_H

#define HOST_NETWORK_SSID "LLED Fan"
#define HOST_NETWORK_PASSWORD "We love LED"
#define WIFI_HOSTNAME "lled.fan"

// Upper cap. Consider the chipset of the LEDs' max framerate:
// WS2812b: 400; WS2813: 2000
#define MAX_FRAMES_PER_SECOND 2000

// See https://github.com/FastLED/FastLED/blob/master/chipsets.h
#define LED_TYPE WS2813
#define COLOR_ORDER GRB
#define LED_PIN 25
#define LED_COUNT 18

// Size of the square artnet-accessible screen
#define VIRTUAL_SCREEN_SIZE 64

// RingResolution = add * x + min
#define CONCENTRIC_RESOLUTION_MIN 4
#define CONCENTRIC_RESOLUTION_ADD 4

// See ROTATION_SENSOR_TYPE_XXX
#define ROTATION_SENSOR_TYPE ROTATION_SENSOR_TYPE_HALL_SYNC
// See ROTATION_EXTRAPOLATION_XXX
#define ROTATION_EXTRAPOLATION ROTATION_EXTRAPOLATION_REGRESSION
// Comma-Separated list of pins in use
#define MAGNET_PINS 33
// If applicable, max framerate of rotation sensor task
#define ROTATION_SENSOR_MS 1

#endif //LED_FAN_SETUP_H
