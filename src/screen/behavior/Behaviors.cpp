//
// Created by Lukas Tenbrink on 10.07.20.
//

#include "Behaviors.h"
#include "CartesianDemo.h"

NativeBehaviors::Dict NativeBehaviors::list = NativeBehaviors::Dict();

void NativeBehaviors::init() {
    list.clear();

    list.put("None", []() {
        return nullptr;
    });
    list.put("Demo", []() {
        return new Demo();
    });
    list.put("StrobeDemo", []() {
        return new StrobeDemo();
    });
    list.put("Strobe", []() {
        return new PerfectStrobe();
    });
    list.put("Dotted", []() {
        return new Dotted();
    });
    list.put("Cartesian Demo", []() {
        return new CartesianDemo();
    });
}
