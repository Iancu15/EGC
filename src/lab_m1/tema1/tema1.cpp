#include "lab_m1/tema1/tema1.h"

#include <stdlib.h>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1() {}


Tema1::~Tema1() {}


void Tema1::Init() {
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // map variables init
    {
        logicSpace.x = 0;       // logic x
        logicSpace.y = 0;       // logic y
        logicSpace.width = 4;   // logic width
        logicSpace.height = 4;  // logic height
        length = 1;
        mapHeight = 4;
        mapWidth = 10;
        mapX = -3;
        mapY = 0;
        translateMinimapOutlX = mapX + 0.001f;
        translateMinimapOutlY = mapY;
    }

    // health bar variables init
    {
        healthBarOX = logicSpace.width / 12;
        healthBarFullOY = logicSpace.height / 4;
        translateHealthBarX = 0;
        translateHealthBarY = 0;
    }

    // score variables init
    {
        scoreOX = logicSpace.width / 28;
        scoreFullOY = logicSpace.height / 12;
        translateScoreX = 0;
        translateScoreY = 0;
    }

    // enemies
    {
        enemySpawnTimer = 0.75f;
        enemySpawnTimerFull = 0.75f;
        visibleEnemies.push_back(Enemy(length / 4, length / 4, 0));
    }

    protag = Player(2 - length / 4, 2 - length / 4, length / 4, length / 4, 1, 0, 0.5f);
    pickUpLength = length / 8;

    Mesh *obstacle = object2D::CreateSquare("Obstacle", glm::vec3(0, 0, 1), length, glm::vec3(0, 0, 1), true);
    Mesh *body = object2D::CreateSquare("body", glm::vec3(0, 0, 1.5f), protag.getLengthOX(), glm::vec3(0, 1, 0), true);
    Mesh *enemyBody = object2D::CreateSquare("enemy_body", glm::vec3(0, 0, 1.5f), length / 4, glm::vec3(1, 0, 0), true);
    Mesh *head = object2D::CreateSquare("head", glm::vec3(0, 0, 2), length / 8, glm::vec3(1, 0, 0), true);
    Mesh *enemyHead = object2D::CreateSquare("enemy_head", glm::vec3(0, 0, 2), length / 8, glm::vec3(0, 0, 0), true);
    Mesh *fist = object2D::CreateSquare("fist", glm::vec3(0, 0, 1.2f), length / 16, glm::vec3(1, 0, 0), true);
    Mesh *projectile = object2D::CreateSquare("projectile", glm::vec3(0, 0, 1), length / 8, glm::vec3(1, 1, 1));
    Mesh *map = object2D::CreateRectangle("map", glm::vec3(0, 0, 0), mapWidth, mapHeight, glm::vec3(0, 0, 0), true);
    Mesh *miniMapOutl = object2D::CreateRectangle("mini_map_outline", glm::vec3(0, 0, 3.5f), mapWidth - 0.01f,mapHeight - 0.01f, glm::vec3(1, 0, 0));
    Mesh *healthBarOutl = object2D::CreateRectangle("health_bar_outline", glm::vec3(0, 0, 3.5f), healthBarOX,healthBarFullOY, glm::vec3(1, 1, 1));
    Mesh *healthBar = object2D::CreateRectangle("health_bar", glm::vec3(0, 0, 3), healthBarOX, healthBarFullOY,glm::vec3(1, 0, 0), true);
    Mesh *scoreOutl = object2D::CreateRectangle("score_outline", glm::vec3(0, 0, 3.5f), scoreOX, scoreFullOY,glm::vec3(1, 1, 1));
    Mesh *scoreBar = object2D::CreateRectangle("score_bar", glm::vec3(0, 0, 3), scoreOX, scoreFullOY,glm::vec3(1, 1, 0), true);
    Mesh *healthPickup = object2D::CreateSquare("health_pickup", glm::vec3(0, 0, 1), pickUpLength, glm::vec3(0, 1, 0), true);
    Mesh *stopTimePickup = object2D::CreateSquare("stoptime_pickup", glm::vec3(0, 0, 1), pickUpLength, glm::vec3(0.625f, 0.125f, 0.9375f), true);
    Mesh *slowTimePickup = object2D::CreateSquare("slowtime_pickup", glm::vec3(0, 0, 1), pickUpLength, glm::vec3(1, 0, 1), true);
    Mesh *fireRatePickup = object2D::CreateSquare("firerate_pickup", glm::vec3(0, 0, 1), pickUpLength, glm::vec3(1, 1, 0), true);

    AddMeshToList(obstacle);
    AddMeshToList(map);
    AddMeshToList(healthBarOutl);
    AddMeshToList(miniMapOutl);
    AddMeshToList(healthBar);
    AddMeshToList(head);
    AddMeshToList(fist);
    AddMeshToList(body);
    AddMeshToList(enemyBody);
    AddMeshToList(enemyHead);
    AddMeshToList(projectile);
    AddMeshToList(scoreOutl);
    AddMeshToList(scoreBar);
    AddMeshToList(healthPickup);
    AddMeshToList(stopTimePickup);
    AddMeshToList(slowTimePickup);
    AddMeshToList(fireRatePickup);

    // obstacles
    {
        // 4 big spawn squares
        {
            obstacles[0] = Obstacle(0, 0, 1, 1);
            obstacles[1] = Obstacle(3, 0, 1, 1);
            obstacles[2] = Obstacle(0, 3, 1, 1);
            obstacles[3] = Obstacle(3, 3, 1, 1);
        }

        // spawn smaller squares
        {
            obstacles[4] = Obstacle(length, length, 0.5f, 0.5f);
            obstacles[5] = Obstacle(length * 1.5f, length * 1.5f, 0.3f, 0.3f);
            obstacles[6] = Obstacle(4 - length * 1.5f, 4 - length * 1.5f, 0.5f, 0.5f);
            obstacles[7] = Obstacle(4 - length * 1.8f, 4 - length * 1.8f, 0.3f, 0.3f);
        }

        // spawn gates
        {
            obstacles[8] = Obstacle(0, length, 0.2f, 0.75f);
            obstacles[9] = Obstacle(0, 4 - length * 1.75f, 0.2f, 0.75f);
            obstacles[10] = Obstacle(4 - length * 0.2f, length, 0.2f, 0.75f);
            obstacles[11] = Obstacle(4 - length * 0.2f, 4 - length * 1.75f, 0.2f, 0.75f);
        }

        // right room obstacles
        {
            obstacles[12] = Obstacle(4 + length * 0.5f, length, 0.75f, 2);
            obstacles[13] = Obstacle(4 + length * 1.75f, 4 - length * 1.5f, 0.5f, 1.5f);
            obstacles[14] = Obstacle(4 + length * 1.75f, 0, 0.5f, 1.5f);
            obstacles[15] = Obstacle(7 - length * 0.3f, 4 - length * 1.5f, 0.3f, 0.3f);
            obstacles[16] = Obstacle(7 - length * 0.3f, length * 1.2f, 0.3f, 0.3f);
        }

        // left room obstacles
        {
            obstacles[17] = Obstacle(-length * 1.25f, length, 0.75f, 2);
            obstacles[18] = Obstacle(-length * 2.25f, 4 - length * 1.5f, 0.5f, 1.5f);
            obstacles[19] = Obstacle(-length * 2.25f, 0, 0.5f, 1.5f);
            obstacles[20] = Obstacle(mapX, 4 - length * 1.5f, 0.3f, 0.3f);
            obstacles[21] = Obstacle(mapX, length * 1.2f, 0.3f, 0.3f);
        }
    }

    // pickups
    {
        for (int i = 0; i < NUM_OF_HEALTH_PICKUPS; i++) {
            pickups[i] = Pickup(pickUpLength, HEALTH, 0);
        }

        int startSlowTime = NUM_OF_HEALTH_PICKUPS;
        for (int i = startSlowTime; i < startSlowTime + NUM_OF_SLOWTIME_PICKUPS; i++) {
            pickups[i] = Pickup(pickUpLength, SLOWTIME, 5);
        }

        int startStopTime = startSlowTime + NUM_OF_SLOWTIME_PICKUPS;
        for (int i = startStopTime; i < startStopTime + NUM_OF_STOPTIME_PICKUPS; i++) {
            pickups[i] = Pickup(pickUpLength, STOPTIME, 2);
        }

        int startFireRate = startStopTime + NUM_OF_STOPTIME_PICKUPS;
        for (int i = startFireRate; i < startFireRate + NUM_OF_FIRERATE_PICKUPS; i++) {
            pickups[i] = Pickup(pickUpLength, FIRERATEUP, 10);
        }
    }
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear) {
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);
    DrawScene(visMatrix);

    // Health bar
    {
        glm::mat3 healthTranslateMatrix = visMatrix * transform2D::Translate(logicSpace.width * 0.87f + translateHealthBarX, logicSpace.height * 0.03f + translateHealthBarY);
        modelMatrix = healthTranslateMatrix;
        modelMatrix *= transform2D::Scale(1, protag.getHealth());
        RenderMesh2D(meshes["health_bar"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["health_bar_outline"], shaders["VertexColor"], healthTranslateMatrix);
    }

    // Score
    {
        // 100-1000 kill count
        glm::mat3 score3TranslateMatrix = visMatrix * transform2D::Translate(logicSpace.width / 2 + logicSpace.width / 5 + translateScoreX, logicSpace.height / 16 + logicSpace.height / 7 + translateScoreY);
        modelMatrix = score3TranslateMatrix;
        modelMatrix *= transform2D::Scale(1, (float) ((int) std::min(protag.getScore(), 1000) / 100) / 10);
        RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["score_outline"], shaders["VertexColor"], score3TranslateMatrix);

        // 10-100 kill count
        glm::mat3 score2TranslateMatrix = visMatrix * transform2D::Translate(logicSpace.width / 2 + logicSpace.width / 5 + logicSpace.width / 23 + translateScoreX, logicSpace.height / 16 + logicSpace.height / 7 + translateScoreY);
        modelMatrix = score2TranslateMatrix;
        modelMatrix *= transform2D::Scale(1, (float) ((int) (protag.getScore() % 100) / 10) / 10);
        RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["score_outline"], shaders["VertexColor"], score2TranslateMatrix);

        // 1-10 kill count
        glm::mat3 score1TranslateMatrix = visMatrix * transform2D::Translate(logicSpace.width / 2 + logicSpace.width / 5 + logicSpace.width * 2 / 23 + translateScoreX, logicSpace.height / 16 + logicSpace.height / 7 + translateScoreY);
        modelMatrix = score1TranslateMatrix;
        modelMatrix *= transform2D::Scale(1, (float) (protag.getScore() % 10) / 10);
        RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["score_outline"], shaders["VertexColor"], score1TranslateMatrix);
    }

    // Minimap
    {
        ViewportSpace viewSpace = ViewportSpace(resolution.x / 2 + resolution.x / 5, resolution.y / 16, resolution.x / 8, resolution.y / 8);
        SetViewportArea(viewSpace, glm::vec3(0.5f), true);

        // Compute uniform 2D visualization matrix
        visMatrix = glm::mat3(1);
        LogicSpace logicSpaceMinimap = logicSpace;
        logicSpaceMinimap.width = mapWidth;
        logicSpaceMinimap.height = mapHeight;
        logicSpaceMinimap.x = mapX;
        logicSpaceMinimap.y = 0;
        visMatrix *= VisualizationTransf2D(logicSpaceMinimap, viewSpace);
        DrawScene(visMatrix);
        modelMatrix = visMatrix * transform2D::Translate(translateMinimapOutlX, translateMinimapOutlY);
        RenderMesh2D(meshes["mini_map_outline"], shaders["VertexColor"], modelMatrix);
    }

    // do stuff with the visible enemies for the current frame
    int i = 0, size;
    size = visibleEnemies.size();
    std::list<Enemy> remainingVisibleEnemies;
    while (i < size && size > 0) {
        Enemy enemy = visibleEnemies.back();
        visibleEnemies.pop_back();

        // rotates the enemy towards the protagonis
        enemy.setAngular(RotateFromTo(enemy.getCenterOX(), enemy.getCenterOY(), protag.getCenterOX(), protag.getCenterOY()));

        // moves the enemy towards the protagonist
        if (!protag.isItDead() && !protag.didTheTimeStop()) {
            enemy.moveTo(protag.getX(), protag.getY(), deltaTimeSeconds);
        }

        if (protag.didTheTimeSlow()) {
            enemy.slowDown();
        } else {
            enemy.speedBack();
        }

        enemy.rectifyCollisionOutsideOfMap(mapX, mapY, mapWidth, mapHeight);
        protag.collisionEnemy(&enemy, deltaTimeSeconds);
        std::list<Projectile> remainingVisibleProjectiles;
        for (auto projctl : visibleProjectiles) {
            projctl.collisionEnemy(&enemy, &protag);
            if (!projctl.isItVisible()) {
                invisibleProjectiles.push_back(projctl);
            } else {
                remainingVisibleProjectiles.push_back(projctl);
            }
        }

        visibleProjectiles.clear();
        visibleProjectiles = remainingVisibleProjectiles;

        if (enemy.isItDead()) {
            deadEnemies.push_back(enemy);
        } else {
            remainingVisibleEnemies.push_back(enemy);
        }

        i++;
    }

    visibleEnemies.clear();
    visibleEnemies = remainingVisibleEnemies;

    // do stuff with visible projectiles (visible in all space, not only logicSpace)
    i = 0, size = visibleProjectiles.size();
    std::list<Projectile> remainingVisibleProjectiles;
    while (i < size && size > 0) {
        Projectile projctl = visibleProjectiles.back();
        visibleProjectiles.pop_back();

        if (!projctl.isItVisible()) {
            invisibleProjectiles.push_back(projctl);
            i++;
            continue;
        }

        projctl.moveTowardsMouse(deltaTimeSeconds);

        if (protag.didTheTimeSlow()) {
            projctl.slowDown();
        } else {
            projctl.speedBack();
        }

        projctl.collisionOutsideOfMap(protag, mapX, mapY, mapWidth, mapHeight);
        projctl.collisionObstacles(obstacles, protag, NUM_OF_OBSTACLES);

        if (!projctl.isItVisible()) {
            invisibleProjectiles.push_back(projctl);
        } else {
            remainingVisibleProjectiles.push_back(projctl);
        }

        i++;
    }

    visibleProjectiles.clear();
    visibleProjectiles = remainingVisibleProjectiles;

    protag.rectifyCollisionOutsideOfMap(mapX, mapY, mapWidth, mapHeight);
    protag.collisionObstacles(obstacles, NUM_OF_OBSTACLES);
    protag.passTime(deltaTimeSeconds);
    for (int i = 0; i < NUM_OF_PICKUPS; i++) {
        pickups[i].collisionObstacles(obstacles, NUM_OF_OBSTACLES);
        protag.collisionPickup(&pickups[i]);
    }

    if (protag.didTheTimeSlow()) {
        enemySpawnTimer -= deltaTimeSeconds / 3;
    } else {
        enemySpawnTimer -= deltaTimeSeconds;
    }

    if (enemySpawnTimer <= 0 && !protag.didTheTimeStop() && !protag.isItDead()) {
        if (deadEnemies.empty()) {
            visibleEnemies.push_back(Enemy(length / 4, length / 4, 0));
        } else {
            Enemy enemy = deadEnemies.back();
            enemy.spawnIfDead(protag, logicSpace.width, logicSpace.height);
            visibleEnemies.push_back(enemy);
            deadEnemies.pop_back();
        }

        enemySpawnTimer = enemySpawnTimerFull;
    }
}


void Tema1::FrameEnd()
{
}

void Tema1::DrawScene(glm::mat3 visMatrix) {
    modelMatrix = visMatrix * transform2D::Translate(mapX, mapY);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

    // Pickups
    {
        for (int i = 0; i < NUM_OF_HEALTH_PICKUPS; i++) {
            modelMatrix = visMatrix * transform2D::Translate(pickups[i].getX(), pickups[i].getY());
            RenderMesh2D(meshes["health_pickup"], shaders["VertexColor"], modelMatrix);
        }

        int startSlowTime = NUM_OF_HEALTH_PICKUPS;
        for (int i = startSlowTime; i < startSlowTime + NUM_OF_SLOWTIME_PICKUPS; i++) {
            modelMatrix = visMatrix * transform2D::Translate(pickups[i].getX(), pickups[i].getY());
            RenderMesh2D(meshes["slowtime_pickup"], shaders["VertexColor"], modelMatrix);
        }

        int startStopTime = startSlowTime + NUM_OF_SLOWTIME_PICKUPS;
        for (int i = startStopTime; i < startStopTime + NUM_OF_STOPTIME_PICKUPS; i++) {
            modelMatrix = visMatrix * transform2D::Translate(pickups[i].getX(), pickups[i].getY());
            RenderMesh2D(meshes["stoptime_pickup"], shaders["VertexColor"], modelMatrix);
        }

        int startFireRate = startStopTime + NUM_OF_STOPTIME_PICKUPS;
        for (int i = startFireRate; i < startFireRate + NUM_OF_FIRERATE_PICKUPS; i++) {
            modelMatrix = visMatrix * transform2D::Translate(pickups[i].getX(), pickups[i].getY());
            RenderMesh2D(meshes["firerate_pickup"], shaders["VertexColor"], modelMatrix);
        }
    }

    // Protagonist
    {
        modelMatrix = visMatrix * transform2D::Translate(protag.getCenterOX(), protag.getCenterOY());
        modelMatrix *= transform2D::Rotate(protag.getAngular());
        glm::mat3 rotateMatrix = modelMatrix;
        modelMatrix = rotateMatrix * transform2D::Translate(-length / 8, -length / 8);
        RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 13 - length / 8, length / 10 - length / 8);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 32 - length / 8, -length / 20 - length / 8);
        RenderMesh2D(meshes["fist"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 7 - length / 8, -length / 20 - length / 8);
        RenderMesh2D(meshes["fist"], shaders["VertexColor"], modelMatrix);
    }

    // draw enemies
    for (list<Enemy>::iterator it = visibleEnemies.begin(); it != visibleEnemies.end(); it++) {
        Enemy enemy = *it;
        modelMatrix = visMatrix * transform2D::Translate(enemy.getCenterOX(), enemy.getCenterOY());
        modelMatrix *= transform2D::Rotate(enemy.getAngular());
        glm::mat3 rotateMatrix = modelMatrix;
        modelMatrix = rotateMatrix * transform2D::Translate(-length / 8, -length / 8);
        RenderMesh2D(meshes["enemy_body"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 13 - length / 8, length / 10 - length / 8);
        RenderMesh2D(meshes["enemy_head"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 32 - length / 8, -length / 20 - length / 8);
        RenderMesh2D(meshes["fist"], shaders["VertexColor"], modelMatrix);

        modelMatrix = rotateMatrix * transform2D::Translate(length / 7 - length / 8, -length / 20 - length / 8);
        RenderMesh2D(meshes["fist"], shaders["VertexColor"], modelMatrix);
    }

    // draw projectiles
    for (list<Projectile>::iterator it = visibleProjectiles.begin(); it != visibleProjectiles.end(); it++) {
        Projectile projctl = *it;
        modelMatrix = visMatrix * transform2D::Translate(protag.getCenterOX(), protag.getCenterOY());
        modelMatrix *= transform2D::Translate(projctl.getX(), projctl.getY());
        modelMatrix *= transform2D::Rotate(projctl.getAngular());
        int projctlScale = projctl.isItVisible() ? 1 : 0;
        modelMatrix *= transform2D::Scale(projctlScale, projctlScale);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }

    // draw obstacles
    for (int i = 0; i < NUM_OF_OBSTACLES; i++) {
        Obstacle obstacle = obstacles[i];
        modelMatrix = visMatrix * transform2D::Translate(obstacle.getX(), obstacle.getY());
        modelMatrix *= transform2D::Scale(obstacle.getLengthOX(), obstacle.getLengthOY());
        RenderMesh2D(meshes["Obstacle"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::MoveWithCamera(float *x, float *y, float dist) {
    if (window->KeyHold(GLFW_KEY_W)) {
        *y += dist;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        *y -= dist;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        *x -= dist;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        *x += dist;
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::MoveCoord(float *ObstacleX, float *ObstacleY, float distX, float distY) {
    *ObstacleX += distX;
    *ObstacleY += distY;
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {
    if (protag.isItDead() == false) {
        float dist = protag.getSpeed() * deltaTime;
        MoveWithCamera(&logicSpace.x, &logicSpace.y, dist);
        MoveWithCamera(&translateHealthBarX, &translateHealthBarY, dist);
        MoveWithCamera(&translateScoreX, &translateScoreY, dist);

        bool W = window->KeyHold(GLFW_KEY_W);
        bool S = window->KeyHold(GLFW_KEY_S);
        bool A = window->KeyHold(GLFW_KEY_A);
        bool D = window->KeyHold(GLFW_KEY_D);
        protag.moveWithCamera(W, S, A, D, dist);

        float distX = 0, distY = 0;
        protag.centerProtag(logicSpace.x, logicSpace.y, 0, 0, &distX, &distY);
        MoveCoord(&logicSpace.x, &logicSpace.y, distX, distY);
        MoveCoord(&translateHealthBarX, &translateHealthBarY, distX, distY);
        MoveCoord(&translateScoreX, &translateScoreY, distX, distY);
    }
}


void Tema1::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_ENTER && protag.isItDead()) {
        protag.revive();
        visibleEnemies.clear();
        enemySpawnTimer = 0;
        logicSpace.x = 0;
        logicSpace.y = 0;
        translateHealthBarX = 0;
        translateHealthBarY = 0;
        translateScoreX = 0;
        translateScoreY = 0;
    }

    if (key == GLFW_KEY_ESCAPE) {
        exit(0);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

float Tema1::RotateFromTo(float fromX, float fromY, float toX, float toY) {
    float oppositeSide = toX - fromX;
    float neighborSide = fromY - toY;
    float hypotenuse = sqrt(oppositeSide * oppositeSide + neighborSide * neighborSide);
    float angular = asin(oppositeSide / hypotenuse);
    if (neighborSide < 0) {
        angular = PI - angular;
    }

    return angular;
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (protag.isItDead() == false) {
        float logicMouseX = mouseX * logicSpace.width / viewSpace.width;
        float logicMouseY = logicSpace.height - mouseY * logicSpace.height / viewSpace.height;
        float logicProtagCenterX = protag.getSpawnX() + protag.getLengthOX();
        float logicProtagCenterY = protag.getSpawnY() + protag.getLengthOY();
        protag.setAngular(RotateFromTo(logicProtagCenterX, logicProtagCenterY, logicMouseX, logicMouseY));
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (!protag.isItDead()) {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && protag.canItFire()) {
            Projectile projctl;
            if (invisibleProjectiles.empty()) {
                projctl = Projectile(length / 8, length / 8, 1.25f, 0, 2);
            } else {
                projctl = invisibleProjectiles.back();
                invisibleProjectiles.pop_back();
            }

            projctl.launchProjectile(protag.getAngular());
            visibleProjectiles.push_back(projctl);
            protag.disableFire();
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
