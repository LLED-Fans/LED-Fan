//
// Created by Lukas Tenbrink on 21.01.20.
//

#include <SPIFFS.h>
#include "TextFiles.h"

bool TextFiles::write(String path, String s) {
    File file = SPIFFS.open(path, FILE_WRITE);

    if(!file){
        Serial.print("There was an error opening the file: ");
        Serial.println(path);
        return false;
    }

    file.print(s);
    file.close();

    return true;
}

String TextFiles::read(String path) {
    File file = SPIFFS.open(path, FILE_READ);

    if(!file){
        return "";
    }

    String string = file.readString();
    file.close();
    return string;
}
