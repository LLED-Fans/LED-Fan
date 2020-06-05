//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Logger.h"

#include <utility>

WifiLogger Logger = WifiLogger(1024);

WifiLogger::WifiLogger(int bufferSize) : data(new CharRoller(bufferSize)) {}

void WifiLogger::println(char *value) {
    data->push(value);
    data->push('\n');
}

void WifiLogger::println(String value) {
    data->push(std::move(value));
    data->push('\n');
}

void WifiLogger::clear() {
    data->clear();
}

String WifiLogger::string() {
    return data->toString();
}

void WifiLogger::println(bool v) { println(String(v ? "true" : "false")); }

void WifiLogger::println(unsigned int v) { println(String(v)); }

void WifiLogger::println(int v) { println(String(v)); }

void WifiLogger::println(long v) { println(String(v)); }

void WifiLogger::println(unsigned long v) { println(String(v)); }

void WifiLogger::println(float v) { println(String(v)); }

void WifiLogger::println(double v) { println(String(v)); }
