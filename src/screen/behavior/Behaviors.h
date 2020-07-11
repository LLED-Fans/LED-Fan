//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_BEHAVIORS_H
#define LED_FAN_BEHAVIORS_H

#include <WString.h>
#include <functional>
#include "NativeBehavior.h"
#include "Ping.h"
#include "StrobeDemo.h"
#include "Dotted.h"
#include "PerfectStrobe.h"
#include "Demo.h"
#include "util/LinearMap.h"

class NativeBehaviors {
public:
    typedef std::function<NativeBehavior*()> Provider;
    typedef LinearMap<String, Provider> Dict;

    static Dict list;

    static void init();;
};

#endif //LED_FAN_BEHAVIORS_H
