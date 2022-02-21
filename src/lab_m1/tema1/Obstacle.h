//
// Created by alex on 06.11.2021.
//

#ifndef GFXFRAMEWORK_obstacle_H
#define GFXFRAMEWORK_obstacle_H

#include "ObjectGame.h"

class Obstacle : public ObjectGame {
    public:
        Obstacle(float X, float Y, float lengthOX, float lengthOY) : ObjectGame(X, Y, lengthOX, lengthOY, 0, 0) {}
        Obstacle() : ObjectGame() {}
        ~Obstacle() = default;
};


#endif //GFXFRAMEWORK_obstacle_H
