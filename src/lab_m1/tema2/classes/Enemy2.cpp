//
// Created by alex on 30.11.2021.
//

#include "Enemy2.h"

void Enemy2::die() {
    this->isDead = true;
    this->isCurrentlyDisplaced = true;
}

void Enemy2::slowDown() {
    if (this->isSlowed == false) {
        this->speed /= 3;
        this->isSlowed = true;
    }
}

void Enemy2::speedBack() {
    if (this->isSlowed) {
        this->speed *= 3;
        this->isSlowed = false;
    }
}

bool Enemy2::isItDead() {
    return isDead;
}

void Enemy2::move(float deltaTimeSeconds, float pi) {
    float distance = deltaTimeSeconds * speed;
    switch (moveCycleStage) {
        case 0:
            this->X += distance;
            this->Z += distance;
            this->angular = pi / 4;
            break;

        case 1:
            this->X += distance;
            this->Z -= distance;
            this->angular = -pi / 4;
            break;

        case 2:
            this->X -= distance * 1.5f;
            this->angular = pi / 2;
            break;
    }
}

void Enemy2::checkForNextStage(Obstacle2 obstacle, float deltaTimeSeconds) {
    if (this->X < this->spawnX || this->Z < this->spawnZ ||
        this->X > this->spawnX + obstacle.getLengthOX() ||
        this->Z > this->spawnZ + obstacle.getLengthOZ()) {
        this->X = this->spawnX;
        this->Z = this->spawnZ;
        moveCycleStage = 0;
        return;
    }

    if (moveCycleStage == 0) {
        if (this->X - this->spawnX > obstacle.getLengthOX() - scale * 1.5f) {
            moveCycleStage = 2;
        }
    } else if (moveCycleStage == 1) {
        if (this->X - this->spawnX > obstacle.getLengthOX() - scale * 1.5f) {
            moveCycleStage = 2;
        }
    } else {
        if (abs(this->X - this->spawnX) < deltaTimeSeconds * 20) {
            if (abs(this->Z - this->spawnZ) < deltaTimeSeconds * 20) {
                moveCycleStage = 0;
            } else {
                moveCycleStage = 1;
            }
        }
    }
}

float Enemy2::getScale() {
    return scale;
}

void Enemy2::passTime(float deltaTimeSeconds) {
    if (this->isCurrentlyDisplaced) {
        this->displacementTimer += deltaTimeSeconds;
        if (this->displacementTimer > this->displacementTime) {
            this->isCurrentlyDisplaced = false;
        }
    }
}

bool Enemy2::isItCurrentlyDisplaced() {
    return this->isCurrentlyDisplaced;
}

float Enemy2::getDisplacementTimer() {
    return this->displacementTimer;
}