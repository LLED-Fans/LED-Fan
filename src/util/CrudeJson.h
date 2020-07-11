//
// Created by Lukas Tenbrink on 11.07.20.
//

#ifndef LED_FAN_CRUDEJSON_H
#define LED_FAN_CRUDEJSON_H


#include <WString.h>
#include <vector>

class CrudeJson {
public:
    static String array(std::vector<String> elements);
};


#endif //LED_FAN_CRUDEJSON_H
