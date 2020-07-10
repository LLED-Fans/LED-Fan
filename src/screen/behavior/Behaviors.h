//
// Created by Lukas Tenbrink on 10.07.20.
//

#ifndef LED_FAN_BEHAVIORS_H
#define LED_FAN_BEHAVIORS_H

#include <map>
#include <WString.h>
#include <functional>
#include "NativeBehavior.h"
#include "Ping.h"
#include "StrobeDemo.h"
#include "Dotted.h"
#include "PerfectStrobe.h"
#include "Demo.h"

class NativeBehaviors {
public:
    typedef std::function<NativeBehavior*()> Provider;
    typedef std::pair<String, Provider> Entry;

    static std::map<String, Provider> list;

    static void add(const String& name, const Provider& provider) {
        list.insert(Entry(name, provider));
    }

    static void init() {
        list.clear();

        add("Demo", []() {
            return new Demo();
        });
        add("StrobeDemo", []() {
            return new StrobeDemo();
        });
        add("Strobe", []() {
            return new PerfectStrobe();
        });
        add("Dotted", []() {
            return new Dotted();
        });
    };
};

#endif //LED_FAN_BEHAVIORS_H
