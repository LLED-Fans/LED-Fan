//
// Created by Lukas Tenbrink on 17.04.20.
//

#ifndef LED_FAN_SPEEDCONTROL_H
#define LED_FAN_SPEEDCONTROL_H


class SpeedControl {
public:
    SpeedControl();

    void setDesiredSpeed(float speed);
private:
    float currentSpeed = 0;
};


#endif //LED_FAN_SPEEDCONTROL_H
