//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Logger.h"

CharRoller *Logger::data = new CharRoller(1024);

void Logger::println(char *value) {
    data->append(value);
    data->append("\n", 1);
}

void Logger::clear() {

}

String Logger::string() {
    return data->toString();
}
