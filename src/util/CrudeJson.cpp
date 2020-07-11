//
// Created by Lukas Tenbrink on 11.07.20.
//

#include "CrudeJson.h"

String CrudeJson::array(std::vector<String> elements) {
    String json = "[";

    bool first = true;
    for (const String& s : elements) {
        if (!first) {
            json += ",";
        }

        first = false;
        json += '"' + s + '"';
    }
    json += "]";

    return json;
}
