//
// Created by alex on 06.11.2021.
//

#include <cstdlib>
#include "Enemy.h"
#include "Player.h"

void Enemy::spawnIfDead(Player protag, float logicSpaceWidth, float logicSpaceHeight) {
    if (this->isDead) {
        this->generateSpeed();
        this->isSlowed = false;
        int side = rand() % 4 + 1;
        float logicEnemyXY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4));
        // left side
        if (side == 1) {
            this->X = protag.getX() - (logicSpaceWidth - protag.getSpawnX());
            if (logicEnemyXY > protag.getSpawnY()) {
                this->Y = protag.getY() + (logicEnemyXY - protag.getSpawnY());
            } else {
                this->Y = protag.getY() - (protag.getSpawnY() - logicEnemyXY);
            }
        }

        // right side
        if (side == 2) {
            this->X = protag.getX() + (logicSpaceWidth - protag.getSpawnX());
            if (logicEnemyXY > protag.getSpawnY()) {
                this->Y = protag.getY() + (logicEnemyXY - protag.getSpawnY());
            } else {
                this->Y = protag.getY() - (protag.getSpawnY() - logicEnemyXY);
            }
        }

        // top side
        if (side == 3) {
            this->Y = protag.getY() + (logicSpaceHeight - protag.getSpawnY());
            if (logicEnemyXY > protag.getSpawnX()) {
                this->X = protag.getX() + (logicEnemyXY - protag.getSpawnX());
            } else {
                this->X = protag.getX() - (protag.getSpawnX() - logicEnemyXY);
            }
        }

        // bottom side
        if (side == 4) {
            this->Y = protag.getY() - (logicSpaceHeight - protag.getSpawnY());
            if (logicEnemyXY > protag.getSpawnX()) {
                this->X = protag.getX() + (logicEnemyXY - protag.getSpawnX());
            } else {
                this->X = protag.getX() - (protag.getSpawnX() - logicEnemyXY);
            }
        }

        this->isDead = false;
    }
}

void Enemy::die() {
    this->isDead = true;
}

void Enemy::generateSpeed() {
    this->speed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.75f)) + 0.5f;
}

void Enemy::slowDown() {
    if (this->isSlowed == false) {
        this->speed /= 3;
        this->isSlowed = true;
    }
}

void Enemy::speedBack() {
    if (this->isSlowed) {
        this->speed *= 3;
        this->isSlowed = false;
    }
}

bool Enemy::isItDead() {
    return isDead;
}