//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_LOGGER_H
#define LED_FAN_LOGGER_H


#include "CharRoller.h"

class WifiLogger {
public:
    CharRoller *data;

    WifiLogger(int bufferSize);

    String string();

    void clear();
    void println(char *value);
    void println(String value);

    void println(bool v);
    void println(unsigned int v);
    void println(int v);
    void println(long v);
    void println(unsigned long v);
    void println(float v);
    void println(double v);
};

extern WifiLogger Logger;


#endif //LED_FAN_LOGGER_H
