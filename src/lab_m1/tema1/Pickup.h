//
// Created by alex on 10.11.2021.
//

#ifndef GFXFRAMEWORK_PICKUP_H
#define GFXFRAMEWORK_PICKUP_H

#include "ObjectGame.h"
#include "Obstacle.h"

enum PickupType { HEALTH, STOPTIME, SLOWTIME, FIRERATEUP };

/**
 * HEALTH: primeste viata GREEN
 * STOPTIME: inamicii se opresc in loc pentru un timp PURPLE
 * SLOWTIME: inamicii, proiectilele si jucatorul se misca mai incet pentru un timp MAGENTA
 * FIRERATEUP: Dubleaza fire rate-ul (timpul de asteptare e injumatatit) YELLOW
 */

class Pickup : public ObjectGame {
public:
    Pickup(float length, PickupType type, float timer) : ObjectGame(-1, -1, length, length, 0, 0) {
        this->type = type;
        this->generateCoord();
        this->timer = timer;
    }

    Pickup() : ObjectGame() {}
    ~Pickup() = default;

    void collisionObstacles(Obstacle *obstacles, int numOfObstacles);
    void generateCoord();
    PickupType getType();
    float getTimer();

private:
    PickupType type;
    float timer;
};


#endif //GFXFRAMEWORK_PICKUP_H
