//
// Created by alex on 06.11.2021.
//

#ifndef GFXFRAMEWORK_PROJECTILE_H
#define GFXFRAMEWORK_PROJECTILE_H


#include "ObjectGame.h"
#include "Obstacle.h"
#include "Enemy.h"

class Projectile: public ObjectGame {
    private:
        bool isVisible, isSlowed;
        float maxDistance;

    public:
        Projectile(float lengthOX, float lengthOY, float speed, float angular, float maxDist) : ObjectGame(0, 0, lengthOX, lengthOY, speed, angular) {
            this->isVisible = false;
            this->maxDistance = maxDist;
        }

        Projectile() : ObjectGame() {}
        ~Projectile() = default;
        void moveTowardsMouse(float dist);
        void launchProjectile(float protagAngular);

        void collisionObstacles(Obstacle *obstacles, Player protag, int numOfObstacles);
        void collisionOutsideOfMap(Player protag, float mapX, float mapY, float mapLengthOX, float mapLengthOY);
        bool isItVisible();
        bool checkCollision(ObjectGame obstacle, Player protag);

        void collisionEnemy(Enemy *enemy, Player *protag);

        void slowDown();
        void speedBack();

        bool checkCollisionEnemy(Enemy enemy, Player protag);
};


#endif //GFXFRAMEWORK_PROJECTILE_H
