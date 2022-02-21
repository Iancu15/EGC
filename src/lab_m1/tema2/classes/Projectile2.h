//
// Created by alex on 01.12.2021.
//

#ifndef GFXFRAMEWORK_PROJECTILE2_H
#define GFXFRAMEWORK_PROJECTILE2_H


#include "ObjectGame2.h"
#include "glm/vec3.hpp"
#include "Enemy2.h"
#include "Pickup2.h"

class Projectile2: public ObjectGame2 {
    private:
        bool isVisible, isSlowed;
        float maxDistance, traveledDistance;
        glm::vec3 forward, position;

    public:
        Projectile2(float diameter, float speed, float angular,
                    float maxDist) : ObjectGame2(0, 0, 0, diameter, diameter, diameter, speed, angular, std::make_pair(0, 0)) {
            this->isVisible = false;
            this->isSlowed = false;
            this->maxDistance = maxDist;
        }

        Projectile2() : ObjectGame2() {}
        ~Projectile2() = default;
        void moveTowardsTarget(float deltaTimeSeconds);
        void launchProjectile(glm::vec3 forward, glm::vec3 position);
        bool isItVisible();

    float getX();

    float getY();

    float getZ();

    glm::vec3 getPosition();

    void collisionWithOutsideMap(float mapX, float mapZ, float mapY, float mapLengthOX, float mapLengthOZ, float mapLengthOY);

    bool checkCollision(ObjectGame2 *obj);

    void collisionWithObstacles(std::list<Obstacle2> obstacles);

    void collisionWithPickups(std::list<Pickup2> pickups);

    void disappear();

    bool checkCollisionEnemy(Enemy2 *obj);
};


#endif //GFXFRAMEWORK_PROJECTILE2_H
