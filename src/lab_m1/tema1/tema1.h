#pragma once

#include "components/simple_scene.h"
#include "Obstacle.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Pickup.h"

#define NUM_OF_OBSTACLES 22
#define NUM_OF_HEALTH_PICKUPS 5
#define NUM_OF_STOPTIME_PICKUPS 3
#define NUM_OF_SLOWTIME_PICKUPS 3
#define NUM_OF_FIRERATE_PICKUPS 4
#define NUM_OF_PICKUPS NUM_OF_HEALTH_PICKUPS + NUM_OF_SLOWTIME_PICKUPS + NUM_OF_STOPTIME_PICKUPS + NUM_OF_FIRERATE_PICKUPS
#define PI 3.14159f

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     public:
        Tema1() ;
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        float RotateFromTo(float fromX, float fromY, float toX, float toY);
        void MoveWithCamera(float *x, float *y, float dist);
        void MoveFromTo(float *fromX, float *fromY, float toX, float toY, float speed, float deltaTimeSeconds);

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
        void MoveCoord(float *obstacleX, float *obstacleY, float distX, float distY);

        void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

     protected:
        float length, mapHeight, mapWidth, translateMinimapOutlX, translateMinimapOutlY, mapX, mapY;
        float healthBarFullOY, healthBarOX, translateHealthBarX, translateHealthBarY;
        float scoreOX, scoreFullOY, translateScoreX, translateScoreY, pickUpLength;
        float enemySpawnTimer, enemySpawnTimerFull;
        std::list<Enemy> visibleEnemies;
        std::list<Enemy> deadEnemies;
        std::list<Projectile> visibleProjectiles;
        std::list<Projectile> invisibleProjectiles;
        Obstacle obstacles[NUM_OF_OBSTACLES];
        Player protag;
        Pickup pickups[NUM_OF_PICKUPS];
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        void Collisionobstacles(ObjectGame obstacle);
    };
}   // namespace m1
