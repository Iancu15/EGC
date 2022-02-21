//
// Created by alex on 28.11.2021.
//

#include "ObjectGame2.h"

ObjectGame2::ObjectGame2(float X, float Y, float Z, float lengthOX, float lengthOY, float lengthOZ, float speed,
                         float angular, std::pair<int, int> gridPos) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
    this->lengthOX = lengthOX;
    this->lengthOY = lengthOY;
    this->lengthOZ = lengthOZ;
    this->speed = speed;
    this->angular = angular;
    this->gridPos = gridPos;
    this->spawnX = X;
    this->spawnZ = Z;
}

ObjectGame2::~ObjectGame2() = default;

float ObjectGame2::getX() const {
    return X;
}

float ObjectGame2::getY() const {
    return Y;
}

float ObjectGame2::getZ() const {
    return Z;
}

float ObjectGame2::getLengthOX() const {
    return lengthOX;
}

float ObjectGame2::getLengthOY() const {
    return lengthOY;
}

float ObjectGame2::getLengthOZ() const {
    return lengthOZ;
}

float ObjectGame2::getSpeed() const {
    return speed;
}

float ObjectGame2::getAngular() const {
    return angular;
}

void ObjectGame2::setAngular(float angular) {
    this->angular = angular;
}

std::pair<int, int> ObjectGame2::getGridPos() {
    return gridPos;
}
