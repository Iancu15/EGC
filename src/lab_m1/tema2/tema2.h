#pragma once

#include <lab_m1/tema2/classes/Obstacle2.h>
#include <lab_m1/tema2/classes/Player2.h>
#include <lab_m1/tema2/classes/Enemy2.h>
#include "components/simple_scene.h"
#include "lab_m1/tema2/camera_tema2.h"
#include "lab_m1/lab4/transform3D.h"
#include "lab_m1/tema2/classes/Projectile2.h"
#include "lab_m1/tema2/classes/Pickup2.h"

#define pi 3.14159265
#define enlargePlayground 10
#define gridSize 15

namespace m1 {
    class Tema2 : public gfxc::SimpleScene {
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
        Tema2();
        ~Tema2() override;

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::CameraTema2 *camera{}, *gameCamera, *miniMapCamera;
        glm::mat4 projectionMatrix{};
        glm::vec3 torsoTranslate{};
        float fov{}, left{}, right{}, zNear{}, zFar{}, bottom{}, top{};
        float membersAngle{}, membersAngleSign{}, membersEnemyAngle;
        int **gridMatrix{};
        std::list<Obstacle2> obstacles;
        std::list<Enemy2> enemies;
        std::list<Projectile2> visibleProjectiles;
        std::list<Projectile2> invisibleProjectiles;
        std::list<Pickup2> pickups;
        Player2 player;
        std::pair<float, float> exitPos;
        std::pair<int, int> exitGridPos;
        float exitSignOffsetZ;
        int numberOfSpacesToGoal;
        float timer, initialTime;
        bool isCurrEnemyVertexDisplaced;
        float currEnemyDisplacementTime;

        bool createMaze();

        void makeMazeObstacles();

        void RenderAlignedBox(Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color);

        void printMaze();

        void spawnPlayer();

        std::list<std::pair<int, int>> getViableNeighbors(std::pair<int, int> pos);

        std::pair<int, int> getNextStartPoint();

        std::pair<int, int> getObstacle(std::pair<int, int> pos);

        bool makeExit(int i, int j);

        void spawnEnemiesAndPickups();

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color,
                        implemented::CameraTema2 *camera);

        void DrawScene();

        void DrawHUD();

        void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

        void resetGame();

        void DrawSceneMinimap();

        void DrawGeneralScene();
    };
}
