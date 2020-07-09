//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_LOGGER_H
#define LED_FAN_LOGGER_H


#include <HardwareSerial.h>
#include "CharRoller.h"

template <class LoggingOutput>
class Logger {
public:
    LoggingOutput output;
    Logger(LoggingOutput output) : output(output) {}

    Logger print(char *value);
    Logger print(String value);
    Logger print(char v);
    Logger ln();

    Logger printf(char * fmt, ...);
    Logger print(bool v);
    Logger print(unsigned int v);
    Logger print(int v);
    Logger print(long v);
    Logger print(unsigned long v);
    Logger print(float v);
    Logger print(double v);
};

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

extern Logger<WifiLogger> WifiLog;

class SerialLogger {
public:
    void write(char *value) { Serial.print(value); };
    void write(String value) { Serial.print(value); };
    void write(char value) { Serial.print(value); };
};

extern Logger<SerialLogger> SerialLog;


#endif //LED_FAN_LOGGER_H
