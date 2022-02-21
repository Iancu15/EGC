//
// Created by alex on 02.12.2021.
//

#ifndef GFXFRAMEWORK_PICKUP2_H
#define GFXFRAMEWORK_PICKUP2_H

#include "ObjectGame2.h"
#include "lab_m1/tema1/Pickup.h"

/**
 * HEALTH: primeste viata GREEN
 * STOPTIME: inamicii se opresc in loc pentru un timp PURPLE
 * SLOWTIME: inamicii se misca mai incet pentru un timp MAGENTA
 * FIRERATEUP: Dubleaza fire rate-ul (timpul de asteptare e injumatatit) YELLOW
 */

class Pickup2 : public ObjectGame2 {
private:
    PickupType type;
    float timer{};
    bool isVisible;

public:
    Pickup2(float X, float Z, float baseLength, float lengthOY, PickupType type, float timer,
            std::pair<int, int> gridPos) : ObjectGame2(X, 0, Z, baseLength, lengthOY, baseLength, 0, 0, gridPos) {
        this->type = type;
        this->timer = timer;
        this->isVisible = true;
    }

    Pickup2() : ObjectGame2() {}
    ~Pickup2() = default;

    PickupType getType();
    float getTimer();
    void disappear();
    bool isItVisible();
};


#endif //GFXFRAMEWORK_PICKUP2_H
