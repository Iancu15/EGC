//
// Created by alex on 06.11.2021.
//

#include <cstdio>
#include <complex>
#include "Projectile.h"
#include "utils/glm_utils.h"

void Projectile::moveTowardsMouse(float dist) {
    if (this->isVisible) {
        if (this->angular > -3.14 / 2 && this->angular < 3.14 / 2) {
            this->moveY(cos(this->angular) * dist, -1);
        } else {
            this->moveY(-cos(this->angular) * dist, 1);
        }

        if (this->angular > 0 && this->angular < 3.14) {
            this->moveX(sin(this->angular) * dist, 1);
        } else {
            this->moveX(-sin(this->angular) * dist, -1);
        }

        float totalDistance = sqrt(pow(this->X, 2) + pow(this->Y, 2));
        if (totalDistance > this->maxDistance) {
            this->isVisible = false;
        }
    }
}

void Projectile::launchProjectile(float protagAngular) {
    this->angular = protagAngular;
    this->isVisible = true;
    this->Y = 0;
    this->X = 0;
}

void Projectile::collisionObstacles(Obstacle *obstacles, Player protag, int numOfObstacles) {
    for (int i = 0; i < numOfObstacles; i++) {
        Obstacle obstacle = obstacles[i];
        if (this->checkCollision(obstacle, protag)) {
            this->isVisible = false;
        }
    }
}

bool Projectile::checkCollision(ObjectGame obj, Player protag) {
    float topRightThisX = this->X + protag.getCenterOX();
    float topRightObjX = obj.getX() + obj.getLengthOX();
    float topRightThisY = this->Y +  protag.getCenterOY();
    float topRightObjY = obj.getY() + obj.getLengthOY();
    if (!(topRightThisX >= obj.getX() && topRightThisY >= obj.getY())) {
        return false;
    }

    return topRightObjX >= this->X + protag.getCenterOX() && topRightObjY >= this->Y + protag.getCenterOY();
}

bool Projectile::checkCollisionEnemy(Enemy enemy, Player protag) {
    float topRightThisX = this->X + protag.getX();
    float topRightObjX = enemy.getX() + enemy.getLengthOX();
    float topRightThisY = this->Y +  protag.getY();
    float topRightObjY = enemy.getY() + enemy.getLengthOY();
    if (!(topRightThisX >= enemy.getX() - this->lengthOX && topRightThisY >= enemy.getY() - this->lengthOY)) {
        return false;
    }

    return topRightObjX + 0.2f * this->lengthOX >= this->X + protag.getX() && topRightObjY + 0.2f * this->lengthOY >= this->Y + protag.getY();
}

void Projectile::collisionEnemy(Enemy *enemy, Player *protag) {
    if (this->isVisible && this->checkCollisionEnemy(*enemy, *protag)) {
        enemy->die();
        this->isVisible = false;
        protag->increaseScore();
    }
}

void Projectile::collisionOutsideOfMap(Player protag, float mapX, float mapY, float mapLengthOX, float mapLengthOY) {
    float projectileX = this->X + protag.getX() + protag.getLengthOX();
    float projectileY = this->Y + protag.getY() + protag.getLengthOY();
    if (projectileX <= mapX || mapX + mapLengthOX <= projectileX) {
        this->isVisible = false;
    }

    if (projectileY <= mapY || mapY + mapLengthOY <= projectileY) {
        this->isVisible = false;
    }
}

bool Projectile::isItVisible() {
    return isVisible;
}

void Projectile::slowDown() {
    if (this->isSlowed == false) {
        this->speed /= 3;
        this->isSlowed = true;
    }
}

void Projectile::speedBack() {
    if (this->isSlowed) {
        this->speed *= 3;
        this->isSlowed = false;
    }
}