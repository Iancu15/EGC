//
// Created by alex on 10.11.2021.
//

#include <cstdlib>
#include "Pickup.h"

void Pickup::generateCoord() {
    this->X = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(10 - this->getLengthOX()))) - 3;
    this->Y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(4 - this->getLengthOY())));
}

void Pickup::collisionObstacles(Obstacle *obstacles, int numOfObstacles) {
    for (int i = 0; i < numOfObstacles; i++) {
        Obstacle obstacle = obstacles[i];
        if (this->checkCollision(obstacle)) {
            this->generateCoord();
        }
    }
}

PickupType Pickup::getType() {
    return this->type;
}

float Pickup::getTimer() {
    return timer;
}



