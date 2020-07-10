//
// Created by Lukas Tenbrink on 02.02.20.
//

#ifndef LED_FAN_UPDATER_H
#define LED_FAN_UPDATER_H


static const int UPDATE_TIME_MILLIS = 60 * 1000;

class Updater {
public:
    bool shouldCheckUpdate = false;
    unsigned long checkUntilTimestamp = 0;

    Updater();

    int check();
    void handle();
};


#endif //LED_FAN_UPDATER_H
