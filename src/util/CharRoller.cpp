//
// Created by Lukas Tenbrink on 17.02.20.
//

#include "CharRoller.h"
#include "../../../../.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h"
#include "../../../../.platformio/packages/toolchain-xtensa32/xtensa-esp32-elf/include/string.h"

CharRoller::CharRoller(const int c) : count(c), head(0) {
    data = new char[count + 1];
    data[count] = '\0';
    clear();
}

void CharRoller::push(char c) {
    data[head] = c;
    head = (head + 1) % count;
}

void CharRoller::push(char *d) {
    push(String(d));
}

void CharRoller::push(String data) {
    push(data.begin(), data.length());
}

void CharRoller::push(char *d, unsigned int length) {
    if (length > count) {
        memcpy(data, d + (length - count), count);
        head = 0;
        return;
    }

    int firstCopyCount = _min(length, count - head);
    int leftoverCount = (int) length - firstCopyCount;

    // Copy what we can from head
    memcpy(data + head, d, firstCopyCount);
    head += firstCopyCount;

    if (leftoverCount > 0) {
        // Roll Over
        memcpy(data, d + firstCopyCount, leftoverCount);
        head = leftoverCount;
    }
}

String CharRoller::toString() {
    char tmp = data[head];

    data[head] = '\0';
    String right(data); // 0 to head
    data[head] = tmp;

    // head to end, data[count] is always \0
    return String(data + head) + String(right);
}

void CharRoller::clear() {
    head = 0;
    memset(data, '\0', count);
}
