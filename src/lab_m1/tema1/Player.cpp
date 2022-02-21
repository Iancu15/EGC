//
// Created by alex on 06.11.2021.
//

#include <cstdio>
#include "Player.h"

float Player::getSpawnX() {
    return spawnX;
}

float Player::getSpawnY() {
    return spawnY;
}

void Player::centerProtag(float logicSpaceX, float logicSpaceY, float initialLogicSpaceX, float initialLogicSpaceY
                            ,float *distX, float *distY) {
    float relativeDistCurrX = this->X - logicSpaceX;
    float relativeDistCurrY = this->Y - logicSpaceY;
    float relativeDistInitX = this->getSpawnX() - initialLogicSpaceX;
    float relativeDistInitY = this->getSpawnY() - initialLogicSpaceY;
    *distX += relativeDistCurrX - relativeDistInitX;
    *distY += relativeDistCurrY - relativeDistInitY;
}

void Player::collisionObstacles(Obstacle *obstacles, int numOfObstacles) {
    for (int i = 0; i < numOfObstacles; i++) {
        Obstacle obstacle = obstacles[i];
        if (this->checkCollision(obstacle)) {
            this->rectifyCollision(obstacle);
        }
    }
}

void Player::collisionEnemy(Enemy *enemy, float deltaTimeSeconds) {
    if (this->checkCollision(*enemy) && this->health != 0) {
        enemy->die();
        this->health -= 0.1;

        if (this->health < 0) {
            this->isDead = true;
            this->health = 0;
            printf("=====================================================\n");
            printf("GAME OVER with TOTAL score %d! Press ENTER to RESTART!\n", score);
        }
    }
}

float Player::getHealth() {
    return health;
}

bool Player::isItDead() {
    return isDead;
}

void Player::revive() {
    this->health = 1;
    this->X = this->spawnX;
    this->Y = this->spawnY;
    this->isDead = false;
    this->score = 0;
}

int Player::getScore() {
    return this->score;
}

void Player::increaseScore() {
    if (this->score < 1000) {
        this->score += 1;
    }
}

void Player::collisionPickup(Pickup *pickup) {
    if (this->checkCollision(*pickup)) {
        switch(pickup->getType()) {
            case HEALTH:
                if (this->health < 1) {
                    this->health += 0.2;
                    if (this->health > 1) {
                        this->health = 1;
                    }

                    pickup->generateCoord();
                }

                break;
            case STOPTIME:
                if (this->hasStopTime == false && this->hasSlowTime == false) {
                    this->hasStopTime = true;
                    this->stopTimeTimer = pickup->getTimer();

                    pickup->generateCoord();
                }

                break;
            case SLOWTIME:
                if (this->hasSlowTime == false && this->hasStopTime == false) {
                    this->hasSlowTime = true;
                    this->slowTimeTimer = pickup->getTimer();
                    this->speed /= 3;

                    pickup->generateCoord();
                }

                break;

            case FIRERATEUP:
                if (this->hasFireRateBuff == false) {
                    this->fullFireRateTimer /= 2;
                    this->hasFireRateBuff = true;
                    if (this->canFire == false) {
                        this->fireRateTimer = 0;
                    }

                    this->fireRateBuffTimer = pickup->getTimer();
                    pickup->generateCoord();
                }
        }
    }
}

void Player::passTime(float timeElapsed) {
    if (this->hasSlowTime) {
        this->slowTimeTimer -= timeElapsed;
        if (this->slowTimeTimer <= 0) {
            this->hasSlowTime = false;
            this->speed *= 3;
        }
    }

    if (this->hasStopTime) {
        this->stopTimeTimer -= timeElapsed;
        if (this->stopTimeTimer <= 0) {
            this->hasStopTime = false;
        }
    }

    if (this->canFire == false) {
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

bool Player::didTheTimeStop() {
    return this->hasStopTime;
}

bool Player::didTheTimeSlow() {
    return this->hasSlowTime;
}

void Player::disableFire() {
    this->canFire = false;
    this->fireRateTimer = this->fullFireRateTimer;
}

bool Player::canItFire() {
    return this->canFire;
}
