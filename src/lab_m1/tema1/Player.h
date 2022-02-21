//
// Created by alex on 06.11.2021.
//

#ifndef GFXFRAMEWORK_PLAYER_H
#define GFXFRAMEWORK_PLAYER_H

#include "ObjectGame.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Pickup.h"

#define NUM_OF_PICKUP_BUFF_TYPES 2

class Enemy;

class Player : public ObjectGame {
    private:
        float spawnX, spawnY, health;
        int score;
        bool isDead;

        // buffs and their timers (if timer reaches 0 then the player stops having the buff)
        bool hasStopTime, hasSlowTime, canFire, hasFireRateBuff;
        float stopTimeTimer, slowTimeTimer, fireRateTimer, fullFireRateTimer, fireRateBuffTimer;

    public:
        Player(float X, float Y, float lengthOX, float lengthOY, float speed, float angular, float fireRateTimer) : ObjectGame(X, Y, lengthOX, lengthOY, speed, angular) {
            this->spawnX = X;
            this->spawnY = Y;
            this->XIncreased = false;
            this->YIncreased = false;
            this->health = 1;
            this->isDead = false;
            this->score = 0;
            this->stopTimeTimer = 0;
            this->slowTimeTimer = 0;
            this->fireRateBuffTimer = 0;
            this->fireRateTimer = fireRateTimer;
            this->fullFireRateTimer = fireRateTimer;
            this->hasStopTime = false;
            this->hasSlowTime = false;
            this->hasFireRateBuff = false;
            this->canFire = true;
        }

        Player() : ObjectGame() {}
        ~Player() = default;
        float getSpawnX();
        float getSpawnY();
        void centerProtag(float logicSpaceX, float logicSpaceY, float initialLogicSpaceX, float initialLogicSpaceY
                        ,float *distX, float *distY);

    void collisionObstacles(Obstacle *obstacles, int numOfObstacles);
    void collisionEnemy(Enemy *enemy, float deltaTimeSeconds);
    float getHealth();
    bool isItDead();
    void revive();
    int getScore();
    void increaseScore();
    void passTime(float timeElapsed);
    void collisionPickup(Pickup *pickup);
    bool didTheTimeStop();
    bool didTheTimeSlow();
    void disableFire();
    bool canItFire();
};


#endif //GFXFRAMEWORK_PLAYER_H
