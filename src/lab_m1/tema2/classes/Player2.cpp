//
// Created by alex on 29.11.2021.
//

#include <list>
#include "Player2.h"
#include "lab_m1/tema1/ObjectGame.h"
#include "Projectile2.h"

#define cameraSpeed 2.0f

float Player2::getHealth() {
    return health;
}

bool Player2::isItDead() {
    return isDead;
}

float Player2::getX() const {
    if (attackMode) {
        return this->camera->position.x;
    }

    return this->camera->GetTargetPosition().x + this->humanTranslate.x;
}

float Player2::getZ() const {
    if (attackMode) {
        return this->camera->position.z;
    }

    return this->camera->GetTargetPosition().z + this->humanTranslate.z;
}

float Player2::getCameraCenterX() const {
    if (attackMode) {
        return this->camera->position.x;
    }

    return this->camera->GetTargetPosition().x;
}

float Player2::getCameraCenterZ() const {
    if (attackMode) {
        return this->camera->position.z;
    }

    return this->camera->GetTargetPosition().z;
}

void Player2::passTime(float timeElapsed) {
    if (this->hasSlowTime) {
        this->slowTimeTimer -= timeElapsed;
        if (this->slowTimeTimer <= 0) {
            this->hasSlowTime = false;
            //this->speed *= 3;
        }
    }

    if (this->hasStopTime) {
        this->stopTimeTimer -= timeElapsed;
        if (this->stopTimeTimer <= 0) {
            this->hasStopTime = false;
        }
    }

    if (!this->canFire) {
        this->fireRateTimer -= timeElapsed;
        if (this->fireRateTimer <= 0) {
            this->canFire = true;
        }
    }

    if (this->hasFireRateBuff) {
        this->fireRateBuffTimer -= timeElapsed;
        if (this->fireRateBuffTimer <= 0) {
            this->hasFireRateBuff = false;
            this->fullFireRateTimer *= 2;
        }

    }
}

bool Player2::didTheTimeStop() {
    return this->hasStopTime;
}

bool Player2::didTheTimeSlow() {
    return this->hasSlowTime;
}

void Player2::disableFire() {
    this->canFire = false;
    this->fireRateTimer = this->fullFireRateTimer;
}

bool Player2::canItFire() {
    return this->canFire;
}

bool Player2::getAttackMode() {
    return attackMode;
}

void Player2::changeMode() {
    float dist = distance(glm::vec3(0), humanTranslate + headTranslate);
    if (attackMode) {
        camera->TranslateForward(-dist);
        camera->AddPosition(glm::vec3(0, -0.5f, 0));
    } else {
        camera->TranslateForward(dist);
        camera->AddPosition(glm::vec3(0, 0.5f, 0));
    }

    this->attackMode = !attackMode;
}

void Player2::moveBackwards(float distance) {
    camera->MoveForward(-distance * this->speed * cameraSpeed);
}

void Player2::moveForward(float distance) {
    camera->MoveForward(distance * this->speed * cameraSpeed);
}

void Player2::moveLeft(float distance) {
    camera->TranslateRight(-distance * this->speed * cameraSpeed);
}

void Player2::moveRight(float distance) {
    camera->TranslateRight(distance * this->speed * cameraSpeed);
}

void Player2::goToSpawn(float offsetX, float offsetZ) {
    camera->SetPosition(glm::vec3(this->spawnX + offsetX, 3, this->spawnZ + 0.2f + offsetZ));
}

glm::vec3 Player2::getHumanTranslate() {
    return humanTranslate;
}

void Player2::die() {
    this->isDead = true;
}

bool Player2::checkCollisionWithOutsideMap(float mapZ, float mapLengthOZ) {
    if (this->getZ() + this->lengthOZ <= mapZ || mapZ + mapLengthOZ <= this->getZ() - 3 * this->lengthOZ) {
        return true;
    }

    return false;
}

bool Player2::checkCollision(ObjectGame2 *obj) {
    float offsetThisX = -this->lengthOX / 1.75f;
    float offsetObjX = this->lengthOX / 2.5f;
    float offsetThisZ= -this->lengthOZ / 2;
    float offsetObjZ = this->lengthOZ / 1.75f;
    if (!attackMode) {
        offsetThisZ -= this->lengthOZ * 2.4f;
        offsetObjZ += this->lengthOZ * 3;
    }

    float topRightThisX = this->getX() + this->lengthOX + offsetThisX;
    float topRightObjX = obj->getX() + obj->getLengthOX() + offsetObjX;
    float topRightThisZ = this->getZ() + this->lengthOZ + offsetThisZ;
    float topRightObjZ = obj->getZ() + obj->getLengthOZ() + offsetObjZ;
    if (!(topRightThisX >= obj->getX() && topRightThisZ >= obj->getZ())) {
        return false;
    }

    return topRightObjX >= this->getX() && topRightObjZ >= this->getZ();
}

void Player2::collisionWithEnemies(std::list<Enemy2> enemies, float deltaTimeSeconds) {
    for (Enemy2 & enemy : enemies) {
        if (enemy.isItDead()) {
            continue;
        }

        if (this->checkCollision(&enemy)) {
            this->health -= 3 * deltaTimeSeconds;
        }
    }
}

bool Player2::collisionWithPickup(Pickup2 *pickup) {
    if (this->checkCollision(pickup)) {
        switch(pickup->getType()) {
            case HEALTH:
                if (this->health < 1) {
                    this->health += 0.2;
                    if (this->health > 1) {
                        this->health = 1;
                    }

                    return true;
                }

                return false;
            case STOPTIME:
                if (!this->hasSlowTime) {
                    this->hasStopTime = true;
                    this->stopTimeTimer += pickup->getTimer();

                    return true;
                }

                return false;
            case SLOWTIME:
                if (!this->hasStopTime) {
                    if (!this->hasSlowTime) {
                        this->hasSlowTime = true;
                    }

                    this->slowTimeTimer += pickup->getTimer();

                    return true;
                }

                return false;

            case FIRERATEUP:
                if (!this->hasFireRateBuff) {
                    this->fullFireRateTimer /= 2;
                    this->hasFireRateBuff = true;
                    if (!this->canFire) {
                        this->fireRateTimer = 0;
                    }
                }

                this->fireRateBuffTimer += pickup->getTimer();

                return true;
        }
    }

    return false;
}

void Player2::collisionWithObstacles(std::list<Obstacle2> obstacles, float deltaTimeSeconds, bool W, bool A, bool S, bool D) {
    for (Obstacle2 & obstacle : obstacles) {
        if (this->checkCollision(&obstacle)) {
            if (W) {
                this->moveBackwards(deltaTimeSeconds * 2);
            }

            if (A) {
                this->moveRight(deltaTimeSeconds * 2);
            }

            if (S) {
                this->moveForward(deltaTimeSeconds * 2);
            }

            if (D) {
                this->moveLeft(deltaTimeSeconds * 2);
            }

            break;
        }
    }
}