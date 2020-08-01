//
// Created by Lukas Tenbrink on 01.08.20.
//

#ifndef LED_FAN_DEVICEID_H
#define LED_FAN_DEVICEID_H

#include <util/uuid/UUID.h>
#include <WString.h>
#include <SPIFFS.h>
#include "TextFiles.h"

class DeviceID {
public:
    static void get(uuid_t uuid = nullptr) {
        auto file = SPIFFS.open(CFG_PATH + String("id"), FILE_APPEND);
        file.seek(0);

        uuid_t data;

        if (file.available() != sizeof(uuid_t)) {
            // New UUID
            uuid_generate(data);
            file.write(data, sizeof(uuid_t));
        }
        else {
            file.read(data, sizeof(uuid_t));
        }

        if (uuid != nullptr)
            memcpy(uuid, data, sizeof(uuid_t));

        file.close();
    };

    static String getString() {
        uuid_t uuid;
        get(uuid);

        char uuidString[UUID_STR_LEN];
        uuid_unparse(uuid, uuidString);
        return String(uuidString);
    };
};

#endif //LED_FAN_DEVICEID_H
