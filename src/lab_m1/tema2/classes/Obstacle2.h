//
// Created by alex on 28.11.2021.
//

#ifndef GFXFRAMEWORK_Obstacle2_H
#define GFXFRAMEWORK_Obstacle2_H

#include "ObjectGame2.h"

class Obstacle2 : public ObjectGame2 {
public:
    Obstacle2(float X, float Z, float lengthOX, float lengthOY, std::pair<int, int> gridPos) : ObjectGame2(X, 0, Z, lengthOX, lengthOY, lengthOX, 0, 0, gridPos) {}
    Obstacle2() : ObjectGame2() {}
    ~Obstacle2() = default;
};

#endif //GFXFRAMEWORK_Obstacle2_H