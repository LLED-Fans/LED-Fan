//
// Created by Lukas Tenbrink on 21.01.20.
//

#ifndef LED_FAN_TEXTFILES_H
#define LED_FAN_TEXTFILES_H


#include <WString.h>

class TextFiles {
public:
    static bool write(String path, String s);
    static String read(String path);
};


#endif //LED_FAN_TEXTFILES_H
