//
// Created by alex on 30.11.2021.
//

#ifndef GFXFRAMEWORK_ENEMY2_H
#define GFXFRAMEWORK_ENEMY2_H


#include "ObjectGame2.h"
#include "Player2.h"
#include "Obstacle2.h"

class Player2;

class Enemy2 : public ObjectGame2 {
private:
    bool isDead, isSlowed, isCurrentlyDisplaced;
    int moveCycleStage;
    float scale, displacementTimer, displacementTime;

public:
    Enemy2(float X, float Y, float Z, float lengthOX, float lengthOY, float lengthOZ,
           float speed, float angular, std::pair<int, int> gridPos, float scale,
           float displacementTime) : ObjectGame2(X, Y, Z, lengthOX, lengthOY, lengthOZ, speed, angular, gridPos) {
        this->isDead = false;
        this->isCurrentlyDisplaced = false;
        this->displacementTimer = 0;
        this->displacementTime = displacementTime;
        this->moveCycleStage = 0;
        this->scale = scale;
        this->isSlowed = false;
    }

    Enemy2() : ObjectGame2() {}
    ~Enemy2() = default;

    void die();

    void slowDown();

    void speedBack();

    bool isItDead();

    void move(float deltaTimeSeconds, float pi);

    void checkForNextStage(Obstacle2 obstacle, float deltaTimeSeconds);

    float getScale();

    void passTime(float deltaTimeSeconds);

    bool isItCurrentlyDisplaced();

    float getDisplacementTimer();
};

#endif //GFXFRAMEWORK_ENEMY2_H
