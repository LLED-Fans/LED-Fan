// ==================================================================
// ===================== Constants  =================================
// ==================================================================

// Do the sensor check on the main thread.
#define ROTATION_SENSOR_TYPE_HALL_SYNC 1
// Do the sensor check as an async xtask.
#define ROTATION_SENSOR_TYPE_HALL_XTASK 2
// Let the sensors interrupt the CPU on activation.
#define ROTATION_SENSOR_TYPE_INTERRUPT 3

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
#define LED_PIN 26
#define LED_COUNT 42
// How many additional LEDs we should set to black, just to be safe
#define LED_OVERFLOW_WALL 5

// 0 to 1, at 0.5 half of the pixels will be brightness-adjusted for the rest.
#define LED_BRIGHTNESS_CORRECTION 0.15

// RingResolution = add * x + min
#define CONCENTRIC_RESOLUTION_MIN 2
#define CONCENTRIC_RESOLUTION_ADD 1

// See ROTATION_SENSOR_TYPE_XXX
#define ROTATION_SENSOR_TYPE ROTATION_SENSOR_TYPE_INTERRUPT
// See ROTATION_EXTRAPOLATION_XXX
#define ROTATION_EXTRAPOLATION ROTATION_EXTRAPOLATION_REGRESSION
// Comma-Separated list of pins in use
#define ROTATION_SENSOR_PINS 14,18,23,4
// If true, calculate the time per checkpoint separately (for wobbly fans)
#define ROTATION_SENSOR_SEPARATE_CHECKPOINTS false
// If applicable, max framerate of rotation sensor task
#define ROTATION_SENSOR_MS 1

// Maximum rotation speed accepted.
// Setting this low helps with interpreting sensor activations correctly.
#define MAX_ROTATIONS_PER_SECOND 20
#define MOTOR_ROTATIONS_PER_SECOND 14
// After this idle time, we're considered to be paused.
#define ROTATION_PAUSED_MS 2000

#endif //LED_FAN_SETUP_H
