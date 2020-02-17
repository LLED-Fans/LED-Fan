//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Logger.h"

#include <utility>

CharRoller *Logger::data = new CharRoller(1024);

void Logger::println(char *value) {
    data->append(value);
    data->append('\n');
}

void Logger::println(String value) {
    data->append(std::move(value));
    data->append('\n');
}

void Logger::clear() {

}

String Logger::string() {
    return data->toString();
}

void Logger::println(int v) { println(String(v)); }

void Logger::println(long v) { println(String(v)); }

void Logger::println(float v) { println(String(v)); }

void Logger::println(double v) { println(String(v)); }
