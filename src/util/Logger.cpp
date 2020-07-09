//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "Logger.h"

#include <utility>

Logger<WifiLogger> WifiLog = Logger<WifiLogger>(WifiLogger(1024));

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(char v) {
    output.write(v);
    return *this;
}

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(String v) {
    output.write(v);
    return *this;
}

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(char *v) {
    output.write(v);
    return *this;
}

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::ln() { return print('\n'); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(bool v) { return print(String(v ? "true" : "false")); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(unsigned int v) { return print(String(v)); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(int v) { return print(String(v)); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(long v) { return print(String(v)); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(unsigned long v) { return print(String(v)); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(float v) { return print(String(v)); }

template <class LoggingOutput>
Logger<LoggingOutput> Logger<LoggingOutput>::print(double v) { return print(String(v)); }


// Need to be specifically declared...
template class Logger<WifiLogger>;
