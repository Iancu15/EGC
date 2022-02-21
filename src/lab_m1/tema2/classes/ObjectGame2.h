//
// Created by alex on 28.11.2021.
//

#ifndef GFXFRAMEWORK_OBJECTGAME2_H
#define GFXFRAMEWORK_OBJECTGAME2_H


#include <utility>

class ObjectGame2 {
    public:
        ObjectGame2(float X, float Y, float Z, float lengthOX, float lengthOY, float lengthOZ, float speed, float angular, std::pair<int, int> gridPos);
        ObjectGame2() = default;
        ~ObjectGame2();
        float virtual getX() const;
        float virtual getY() const;
        float virtual getZ() const;
        float getLengthOY() const;
        float getLengthOX() const;
        float getLengthOZ() const;
        std::pair<int, int> getGridPos();
        float getSpeed() const;
        float getAngular() const;
        void setAngular(float angular);
        float getOtherEndOX() const;
        float getOtherEndOY() const;
        float getOtherEndOZ() const;

protected:
    float X, Y, Z, lengthOX, lengthOY, lengthOZ, speed, angular;
    std::pair<int, int> gridPos;
    float spawnX, spawnZ;
};


#endif //GFXFRAMEWORK_OBJECTGAME2_H
