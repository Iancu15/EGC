//
// Created by alex on 06.11.2021.
//

#include "ObjectGame.h"

ObjectGame::ObjectGame(float X, float Y, float lengthOX, float lengthOY, float speed, float angular) {
    this->X = X;
    this->Y = Y;
    this->lengthOX = lengthOX;
    this->lengthOY = lengthOY;
    this->speed = speed;
    this->angular = angular;
    this->XIncreased = false;
    this->YIncreased = false;
}

ObjectGame::~ObjectGame() = default;

float ObjectGame::getX() {
    return X;
}

float ObjectGame::getY() {
    return Y;
}

float ObjectGame::getLengthOX() {
    return lengthOX;
}

float ObjectGame::getLengthOY() {
    return lengthOY;
}

float ObjectGame::getSpeed() {
    return speed;
}

float ObjectGame::getAngular() {
    return angular;
}

float ObjectGame::getCenterOX() {
    return X + lengthOX;
}

float ObjectGame::getCenterOY() {
    return Y + lengthOY;
}

void ObjectGame::moveX(float dist, int dir) {
    this->X += dist * dir * this->speed;
}

void ObjectGame::moveY(float dist, int dir) {
    this->Y += dist * dir * this->speed;
}

void ObjectGame::setAngular(float angular) {
    this->angular = angular;
}

bool ObjectGame::checkCollision(ObjectGame obj) {
    float topRightThisX = this->X + this->lengthOX + this->lengthOX / 2;
    float topRightObjX = obj.X + obj.lengthOX - this->lengthOX / 2;
    float topRightThisY = this->Y + this->lengthOY + this->lengthOY / 2;
    float topRightObjY = obj.Y + obj.lengthOY - this->lengthOY / 2;
    if (!(topRightThisX >= obj.X && topRightThisY >= obj.Y)) {
        return false;
    }

    return topRightObjX >= this->X && topRightObjY >= this->Y;
}

void ObjectGame::moveTo(float toX, float toY, float dist) {
    if (this->X > toX) {
        this->X -= speed * dist;
    } else {
        this->X += speed * dist;
    }

    if (this->Y > toY) {
        this->Y -= speed * dist;
    } else {
        this->Y += speed * dist;
    }
}

void ObjectGame::moveWithCamera(bool W, bool S, bool A, bool D, float dist) {
    if (W) {
        this->Y += dist;
        YIncreased = true;
    }

    if (S) {
        this->Y -= dist;
        YIncreased = false;
    }

    if (A) {
        this->X -= dist;
        XIncreased = false;
    }

    if (D) {
        this->X += dist;
        XIncreased = true;
    }
}

void ObjectGame::rectifyCollisionOutsideOfMap(float mapX, float mapY, float mapLengthOX, float mapLengthOY) {
    if (this->X + this->lengthOX / 2 <= mapX) {
        this->X += mapX - (this->X + this->lengthOX / 2);
    }

    if (mapX + mapLengthOX <= this->X + this->lengthOX * 3 / 2) {
        this->X -= this->X + this->lengthOX * 3 / 2 - (mapX + mapLengthOX);
    }

    if (this->Y + this->lengthOY / 2 <= mapY) {
        this->Y += mapY - (this->Y + this->lengthOY / 2);
    }

    if (mapY + mapLengthOY <= this->Y + this->lengthOY * 3 / 2) {
        this->Y -= this->Y + this->lengthOY * 3 / 2 - (mapY + mapLengthOY);
    }
}

void ObjectGame::rectifyCollision(ObjectGame obj) {
    if (this->X + this->getLengthOX() > obj.getX() && obj.getX() + obj.getLengthOX() > this->X) {
        // bottom obj side
        if (this->Y < obj.getY() && YIncreased) {
            this->Y -= (this->Y + this->lengthOY) - (obj.getY() - this->lengthOY / 2);
        }

        // top obj side
        if (this->Y + this->getLengthOY() > obj.getY() + obj.getLengthOY() && !YIncreased){
            this->Y += (obj.getY() + obj.getLengthOY()) - (this->Y + this->lengthOY / 2);
        }
    }

    if (this->Y + this->getLengthOY() > obj.getY() && obj.getY() + obj.getLengthOY() > this->Y) {
        // left obj side
        if (this->X < obj.getX() && XIncreased) {
            this->X -= (this->X + this->lengthOX) - (obj.getX() - this->lengthOX / 2);
        }

        // right obj side
        if (this->X + this->getLengthOX() > obj.getX() + obj.getLengthOX() && !XIncreased) {
            this->X += (obj.getX() + obj.getLengthOX()) - (this->X + this->lengthOX / 2);
        }
    }
}