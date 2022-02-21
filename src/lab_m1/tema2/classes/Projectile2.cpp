//
// Created by alex on 01.12.2021.
//

#include <list>
#include "Projectile2.h"
#include "utils/glm_utils.h"
#include "Obstacle2.h"
#include "Enemy2.h"
#include "Pickup2.h"

void Projectile2::moveTowardsTarget(float deltaTimeSeconds) {
    float distance = deltaTimeSeconds * speed;
    this->position += glm::normalize(forward) * distance;
    this->traveledDistance += distance;
    if (this->traveledDistance > maxDistance) {
        this->isVisible = false;
    }
}

void Projectile2::launchProjectile(glm::vec3 forward, glm::vec3 position) {
    this->isVisible = true;
    this->forward = forward;
    this->traveledDistance = 0;
    this->position = position;
}

glm::vec3 Projectile2::getPosition() {
    return position;
}

bool Projectile2::isItVisible() {
    return isVisible;
}

float Projectile2::getX() {
    return position.x;
}

float Projectile2::getY() {
    return position.y;
}

float Projectile2::getZ() {
    return position.z;
}

void Projectile2::collisionWithOutsideMap(float mapX, float mapZ, float mapY, float mapLengthOX, float mapLengthOZ, float mapLengthOY) {
    if (this->getX() - this->lengthOX / 2 <= mapX || mapX + mapLengthOX <= this->getX() + this->lengthOX / 2) {
        this->isVisible = false;
    }

    if (this->getZ() - this->lengthOZ / 2 <= mapZ || mapZ + mapLengthOZ <= this->getZ() + this->lengthOZ / 2) {
        this->isVisible = false;
    }

    if (this->getY() - this->lengthOY / 2 <= mapY || mapY + mapLengthOY <= this->getY() + this->lengthOY / 2) {

    }
}

bool Projectile2::checkCollision(ObjectGame2 *obj) {
    float X = std::max(obj->getX(), std::min(this->getX(), obj->getX() + obj->getLengthOX()));
    float Y = std::max(obj->getY(), std::min(this->getY(), obj->getY() + obj->getLengthOY()));
    float Z = std::max(obj->getZ(), std::min(this->getZ(), obj->getZ() + obj->getLengthOZ()));
    float dist = std::sqrt((X - this->getX()) * (X - this->getX()) + (Y - this->getY()) * (Y - this->getY()) +
            (Z - this->getZ()) * (Z - this->getZ()));

    return dist < this->lengthOX / 4;
}

bool Projectile2::checkCollisionEnemy(Enemy2 *obj) {
    float X = std::max(obj->getX(), std::min(this->getX(), obj->getX() + obj->getLengthOX()));
    float Y = std::max(obj->getY(), std::min(this->getY(), obj->getY() + obj->getLengthOY()));
    float Z = std::max(obj->getZ(), std::min(this->getZ(), obj->getZ() + obj->getLengthOZ())) - obj->getLengthOZ();
    float dist = std::sqrt((X - this->getX()) * (X - this->getX()) + (Y - this->getY()) * (Y - this->getY()) +
                           (Z - this->getZ()) * (Z - this->getZ()));

    return dist < this->lengthOX;
}

void Projectile2::collisionWithObstacles(std::list<Obstacle2> obstacles) {
    for (Obstacle2 & obstacle : obstacles) {
        if (this->checkCollision(&obstacle)) {
            this->isVisible = false;
            break;
        }
    }
}

void Projectile2::collisionWithPickups(std::list<Pickup2> pickups) {
    for (Pickup2 & pickup : pickups) {
        if (pickup.isItVisible()) {
            if (this->checkCollision(&pickup)) {
                this->isVisible = false;
                break;
            }
        }
    }
}

void Projectile2::disappear() {
    this->isVisible = false;
}