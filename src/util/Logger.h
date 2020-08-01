//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_LOGGER_H
#define LED_FAN_LOGGER_H


#include <esp_log.h>
#include "CharRoller.h"

static const char* TAG = "lled";

template <class LoggingOutput>
class Logger {
public:
    LoggingOutput output;
    Logger(LoggingOutput output) : output(output) {}

    Logger print(const char *value);
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
    SerialLogger() {
        esp_log_level_set(TAG, ESP_LOG_INFO);
    }

    // TODO Not ideal lol, but hey, let's fix later
    void write(char *value) { ESP_LOGI(TAG, "%s", value); };
    void write(String value) { ESP_LOGI(TAG, "%s", value.begin()); };
    void write(char value) { ESP_LOGI(TAG, "%c", value); };
};

extern Logger<SerialLogger> SerialLog;


#endif //LED_FAN_LOGGER_H
