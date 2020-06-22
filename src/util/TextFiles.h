//
// Created by Lukas Tenbrink on 21.01.20.
//

#ifndef LED_FAN_TEXTFILES_H
#define LED_FAN_TEXTFILES_H


static const char *const CFG_PATH = "/cfg/";

#include <WString.h>

class TextFiles {
public:
    static bool write(String path, String s);
    static String read(String path);

    static bool writeConf(String path, String s);
    static String readConf(String path);
};


#endif //LED_FAN_TEXTFILES_H
