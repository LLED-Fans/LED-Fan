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

    WifiLogger clear();
    WifiLogger ln();
    
    WifiLogger print(char *value);
    WifiLogger print(String value);

    WifiLogger print(bool v);
    WifiLogger print(unsigned int v);
    WifiLogger print(int v);
    WifiLogger print(long v);
    WifiLogger print(unsigned long v);
    WifiLogger print(float v);
    WifiLogger print(double v);
};

extern WifiLogger Logger;


#endif //LED_FAN_LOGGER_H
