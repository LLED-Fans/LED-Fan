//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_LOGGER_H
#define LED_FAN_LOGGER_H


#include "CharRoller.h"

class WifiLogger {
public:
    CharRoller *data;

    WifiLogger(int bufferSize) : data(new CharRoller(bufferSize)) {};

    String string() { return data->toString(); };
    void clear() { data->clear(); };

    void write(char *value) { data->push(value); };
    void write(String value) { data->push(std::move(value)); };
    void write(char value) { data->push(value); };
};

template <class LoggingOutput>
class Logger {
public:
    LoggingOutput output;
    Logger(LoggingOutput output) : output(output) {}

    Logger print(char *value);
    Logger print(String value);
    Logger print(char v);
    Logger ln();

    Logger print(bool v);
    Logger print(unsigned int v);
    Logger print(int v);
    Logger print(long v);
    Logger print(unsigned long v);
    Logger print(float v);
    Logger print(double v);
};

extern Logger<WifiLogger> WifiLog;


#endif //LED_FAN_LOGGER_H
