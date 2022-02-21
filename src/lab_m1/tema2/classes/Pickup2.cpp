//
// Created by alex on 02.12.2021.
//

#include "Pickup2.h"
#include "lab_m1/tema1/Pickup.h"

PickupType Pickup2::getType() {
    return this->type;
}

float Pickup2::getTimer() {
    return timer;
}

void Pickup2::disappear() {
    this->isVisible = false;
}

bool Pickup2::isItVisible() {
    return this->isVisible;
}