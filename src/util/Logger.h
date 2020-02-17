//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_LOGGER_H
#define LED_FAN_LOGGER_H


#include "CharRoller.h"

class Logger {
public:
    static CharRoller *data;

    static String string();

    static void clear();
    static void println(char *value);
    static void println(String value);

    static void println(int v);
    static void println(long v);
    static void println(float v);
    static void println(double v);
};


#endif //LED_FAN_LOGGER_H
