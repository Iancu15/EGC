//
// Created by alex on 06.11.2021.
//

#ifndef GFXFRAMEWORK_ENEMY_H
#define GFXFRAMEWORK_ENEMY_H

#include "ObjectGame.h"
#include "Player.h"

class Player;

class Enemy : public ObjectGame {
    private:
        bool isDead, isSlowed;

    public:
        Enemy(float lengthOX, float lengthOY, float angular) : ObjectGame(0, 0, lengthOX, lengthOY, 0, angular) {
            this->isDead = true;
            this->generateSpeed();
        }

        Enemy() : ObjectGame() {}
        ~Enemy() = default;
        void spawnIfDead(Player protag, float logicSpaceWidth, float logicSpaceHeight);
        void generateSpeed();

    void die();

    void slowDown();

    void speedBack();

    bool isItDead();
};


#endif //GFXFRAMEWORK_ENEMY_H
