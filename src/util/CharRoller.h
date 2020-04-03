//
// Created by Lukas Tenbrink on 17.02.20.
//

#ifndef LED_FAN_CHARROLLER_H
#define LED_FAN_CHARROLLER_H

#include <WString.h>
#include "RollerIterator.h"

class CharRoller {
public:
    const unsigned int count;
    char *data;

    CharRoller(const int count);

    void clear();

    void push(char c);

    void push(char *data);
    void push(String data);
    void push(char *d, unsigned int length);

    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);

    String toString();

    RollerIterator<char> begin() {return {data, head, count};}
    RollerIterator<char> end() {return {};}
private:
    unsigned int head;
};


#endif //LED_FAN_CHARROLLER_H
