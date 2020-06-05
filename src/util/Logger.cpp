//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Logger.h"

#include <utility>

WifiLogger Logger = WifiLogger(1024);

WifiLogger::WifiLogger(int bufferSize) : data(new CharRoller(bufferSize)) {}

WifiLogger WifiLogger::ln() {
    return print('\n');
}

WifiLogger WifiLogger::print(char *value) {
    data->push(value);
    return *this;
}

WifiLogger WifiLogger::print(String value) {
    data->push(std::move(value));
    return *this;
}

WifiLogger WifiLogger::print(char v) {
    data->push(v);
    return *this;
}

WifiLogger WifiLogger::clear() {
    data->clear();
}

String WifiLogger::string() {
    return data->toString();
}

WifiLogger WifiLogger::print(bool v) { print(String(v ? "true" : "false")); }

WifiLogger WifiLogger::print(unsigned int v) { print(String(v)); }

WifiLogger WifiLogger::print(int v) { print(String(v)); }

WifiLogger WifiLogger::print(long v) { print(String(v)); }

WifiLogger WifiLogger::print(unsigned long v) { print(String(v)); }

WifiLogger WifiLogger::print(float v) { print(String(v)); }

WifiLogger WifiLogger::print(double v) { print(String(v)); }
