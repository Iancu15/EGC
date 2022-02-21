//
// Created by alex on 06.11.2021.
//

#ifndef GFXFRAMEWORK_ObjectGame_H
#define GFXFRAMEWORK_ObjectGame_H

class ObjectGame {
    public:
        ObjectGame(float X, float Y, float lengthOX, float lengthOY, float speed, float angular);
        ObjectGame() {}
        ~ObjectGame();
        float getX();
        float getY();
        float getLengthOY();
        float getLengthOX();
        float getSpeed();
        float getAngular();
        void setAngular(float angular);
        float getCenterOX();
        float getCenterOY();
        void moveX(float dist, int dir);
        void moveY(float dist, int dir);
        bool checkCollision(ObjectGame obj);
        void moveTo(float toX, float toY, float dist);
        void moveWithCamera(bool W, bool S, bool A, bool D, float dist);
        void rectifyCollisionOutsideOfMap(float mapX, float mapY, float mapLengthOX, float mapLengthOY);
        void rectifyCollision(ObjectGame obj);

    protected:
            float X, Y, lengthOX, lengthOY, speed, angular;
            bool XIncreased, YIncreased;
};


#endif //GFXFRAMEWORK_ObjectGame_H
