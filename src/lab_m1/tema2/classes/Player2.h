//
// Created by alex on 29.11.2021.
//

#ifndef GFXFRAMEWORK_PLAYER2_H
#define GFXFRAMEWORK_PLAYER2_H


#include <lab_m1/tema2/camera_tema2.h>
#include <list>
#include "ObjectGame2.h"
#include "Obstacle2.h"
#include "Enemy2.h"
#include "lab_m1/tema1/Pickup.h"
#include "Pickup2.h"

class Enemy2;

class Player2: public ObjectGame2 {
    private:
        float spawnX, spawnZ, health;
        bool isDead, attackMode;
        implemented::CameraTema2 *camera;

        // distance from third person camera to stuff
        glm::vec3 humanTranslate, headTranslate;

        // buffs and their timers (if timer reaches 0 then the player stops having the buff)
        bool hasStopTime, hasSlowTime, canFire, hasFireRateBuff;
        float stopTimeTimer, slowTimeTimer, fireRateTimer, fullFireRateTimer, fireRateBuffTimer;

    public:
        Player2(float spawnX, float spawnZ, float lengthOX, float lengthOY,
                float lengthOZ, float speed, float angular, float fireRateTimer,
                implemented::CameraTema2 *camera, glm::vec3 humanTranslate,
                glm::vec3 headTranslate,
                std::pair<int, int> gridPos) : ObjectGame2(0, 0, 0, lengthOX, lengthOY, lengthOZ, speed, angular, gridPos) {
            this->spawnX = spawnX;
            this->spawnZ = spawnZ;
            this->health = 1;
            this->isDead = false;
            this->stopTimeTimer = 0;
            this->slowTimeTimer = 0;
            this->fireRateBuffTimer = 0;
            this->fireRateTimer = fireRateTimer;
            this->fullFireRateTimer = fireRateTimer;
            this->hasStopTime = false;
            this->hasSlowTime = false;
            this->hasFireRateBuff = false;
            this->canFire = true;
            this->camera = camera;
            this->attackMode = false;
            this->humanTranslate = humanTranslate;
            this->headTranslate = headTranslate;
        }

        Player2() : ObjectGame2() {}
        ~Player2() = default;
        float getHealth();
        bool isItDead();
        glm::vec3 getHumanTranslate();
        void passTime(float timeElapsed);
        bool didTheTimeStop();
        bool didTheTimeSlow();
        void disableFire();
        bool canItFire();
        bool getAttackMode();
        void changeMode();
        void moveForward(float distance);
        void moveBackwards(float distance);
        void moveLeft(float distance);
        void moveRight(float distance);

    float getX() const override;
    float getZ() const override;

    void goToSpawn(float offsetX, float offsetZ);

    float getCameraCenterX() const;

    float getCameraCenterZ() const;

    void die();

    bool checkCollisionWithOutsideMap(float mapZ, float mapLengthOZ);

    bool checkCollision(ObjectGame2 *obj);

    void collisionWithEnemies(std::list<Enemy2> enemies, float deltaTimeSeconds);

    bool collisionWithPickup(Pickup2 *pickup);

    void collisionWithObstacles(std::list<Obstacle2> obstacles, float deltaTimeSeconds, bool W, bool A, bool S, bool D);
};


#endif //GFXFRAMEWORK_PLAYER2_H
