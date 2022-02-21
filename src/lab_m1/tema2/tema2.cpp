#include "lab_m1/tema2/tema2.h"
#include "lab_m1/lab3/object2D.h"
#include "lab_m1/lab3/transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <set>

using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2() {
    window->DisablePointer();

    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // create shader for Player
    {
        auto *shader = new Shader("PlayerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/PlayerVertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/PlayerFragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // create shader for coloring
    {
        auto *shader = new Shader("ColorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/ColorVertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/ColorFragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // create shader for enemies
    {
        auto *shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/EnemyVertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab_m1/tema2/shaders/EnemyFragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // space variables
    {
        fov = RADIANS(60);
        left = -3;
        right = 3;
        bottom = -3;
        top = 3;
        zNear = 0.01f;
        zFar = 400.0f;
        projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    }
}

Tema2::~Tema2() = default;

void Tema2::Init() {
    // set Misc Player variables
    {
        torsoTranslate = glm::vec3(0, 2, -3);
        membersAngle = 0;
        membersAngleSign = 1;
    }

    // set game camera
    {
        gameCamera = new implemented::CameraTema2();
        gameCamera->Set(glm::vec3(0, 3, 0.2f), glm::vec3(0, 2.875f, 0), glm::vec3(0, 1, 0));
    }

    // set minimap camera
    {
        miniMapCamera = new implemented::CameraTema2();
        float mapLength = enlargePlayground * gridSize;
        glm::vec3 position = glm::vec3(mapLength / 2, mapLength / 2, mapLength / 2);
        glm::vec3 forward = glm::vec3(0, -0.999989f, -0.004797f);
        glm::vec3 right = glm::vec3(1, 0, 0.000001f);
        glm::vec3 up = glm::vec3(0.000001f, 0.004797f, -0.999989f);
        miniMapCamera->Set(position, forward, right, up);
    }

    bool wasCreatedSuccessfully = createMaze();
    if (!wasCreatedSuccessfully) {
        resetGame();
        return;
    }

    spawnPlayer();
    makeMazeObstacles();
    spawnEnemiesAndPickups();
    player.goToSpawn(obstacles.back().getLengthOX() / 2, obstacles.back().getLengthOZ() / 2);
    initialTime = (float) numberOfSpacesToGoal * 3;
    timer = initialTime;
    isCurrEnemyVertexDisplaced = false;
}

void Tema2::resetGame() {
    obstacles.clear();
    enemies.clear();
    visibleProjectiles.clear();
    invisibleProjectiles.clear();
    pickups.clear();
    Init();
}

void Tema2::printMaze() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (gridMatrix[i][j] == 1) {
                printf("* ");
            } else if (gridMatrix[i][j] == 0) {
                printf("= ");
            } else if (gridMatrix[i][j] == 3) {
                printf("P ");
            } else if (gridMatrix[i][j] == 2) {
                printf("E ");
            } else if (gridMatrix[i][j] == 4) {
                printf("$ ");
            } else {
                printf("B ");
            }
        }

        printf("\n");
    }

    printf("\n");
}

std::list<std::pair<int, int>> Tema2::getViableNeighbors(std::pair<int, int> pos) {
    int i = pos.first;
    int j = pos.second;
    std::list<std::pair<int, int>> pair_list;

    // North
    if (pos.first > 1 && gridMatrix[i - 1][j] == -1) {
        pair_list.emplace_back(i - 1, j);
    }

    // South
    if (pos.first < gridSize - 2 && gridMatrix[i + 1][j] == -1) {
        pair_list.emplace_back(i + 1, j);
    }

    // West
    if (pos.second > 1 && gridMatrix[i][j - 1] == -1) {
        pair_list.emplace_back(i, j - 1);
    }

    // East
    if (pos.second < gridSize - 2 && gridMatrix[i][j + 1] == -1) {
        pair_list.emplace_back(i , j + 1);
    }

    return pair_list;
}

std::pair<int, int> Tema2::getObstacle(std::pair<int, int> pos) {
    int i = pos.first;
    int j = pos.second;

    // North
    if (pos.first > 1 && gridMatrix[i - 1][j] == 1) {
        return std::make_pair(i - 1, j);
    }

    // South
    if (pos.first < gridSize - 2 && gridMatrix[i + 1][j] == 1) {
        return std::make_pair(i + 1, j);
    }

    // West
    if (pos.second > 1 && gridMatrix[i][j - 1] == 1) {
        return std::make_pair(i, j - 1);
    }

    // East
    if (pos.second < gridSize - 2 && gridMatrix[i][j + 1] == 1) {
        return std::make_pair(i , j + 1);
    }

    return pos;
}

std::pair<int, int> Tema2::getNextStartPoint() {
    for (int i = 1; i < gridSize - 1; i++) {
        for (int j = 1; j < gridSize - 1; j++) {
            if (gridMatrix[i][j] == -1) {
                std::pair<int, int> pair = std::make_pair(i, j);
                if (getObstacle(pair) != pair) {
                    return pair;
                }
            }
        }
    }

    return std::make_pair(-1, -1);
}

bool Tema2::makeExit(int i, int j) {
    gridMatrix[i][j] = 0;
    float X = (gridSize - 1 - (float) i) * enlargePlayground;
    float Z = (float) j * enlargePlayground;
    exitSignOffsetZ = 0;
    if (j == gridSize - 1) {
        Z += enlargePlayground - 0.1f;
        exitSignOffsetZ += 0.05f * enlargePlayground;
    }

    exitGridPos = std::make_pair(i, j);
    exitPos = std::make_pair(X, Z);
    return true;
}

bool Tema2::createMaze() {
    gridMatrix = (int**) malloc(sizeof(int*) * gridSize);
    for (int i = 0; i < gridSize; i++) {
        gridMatrix[i] = (int*) malloc(sizeof(int) * gridSize);

        // fill the row with unvisited cells
        for (int j = 0; j < gridSize; j++) {
            gridMatrix[i][j] = -1;
        }
    }

    // make spanning tree
    std::pair<int, int> initial_pos = std::make_pair(rand() % (gridSize - 2) + 1, rand() % (gridSize - 2) + 1);
    gridMatrix[initial_pos.first][initial_pos.second] = 0;
    std::pair<int, int> curr_pos = initial_pos;
    while (true) {
        std::list<std::pair<int, int>> viable_neighbors = getViableNeighbors(curr_pos);
        if (viable_neighbors.empty()) {
            std::pair<int, int> next_pos = getNextStartPoint();
            if (next_pos.first == -1) {
                break;
            }

            gridMatrix[next_pos.first][next_pos.second] = 0;
            std::pair<int, int> obstacle = getObstacle(next_pos);
            gridMatrix[obstacle.first][obstacle.second] = 0;
            curr_pos = next_pos;
            continue;
        }

        int next_pos_index = rand() % viable_neighbors.size();
        int index = 0;
        for (std::pair<int, int> & neighbor_pos : viable_neighbors) {
            if (index == next_pos_index) {
                curr_pos = neighbor_pos;
                gridMatrix[neighbor_pos.first][neighbor_pos.second] = 0;
            } else {
                gridMatrix[neighbor_pos.first][neighbor_pos.second] = 1;
            }

            index++;
        }
    }

    // make outer walls and exit
    {
        bool madeExit = false;

        // left and right walls
        for (int i = 0; i < gridSize; i++) {
            gridMatrix[i][0] = 1;
            if (!madeExit && gridMatrix[i][1] == 0) {
                madeExit = makeExit(i, 0);
            }

            gridMatrix[i][gridSize - 1] = 1;
            if (!madeExit && gridMatrix[i][gridSize - 2] == 0) {
                madeExit = makeExit(i, gridSize - 1);
            }
        }

        if (!madeExit) {
            return false;
        }

        // top and bottom walls
        for (int j = 0; j < gridSize; j++) {
            gridMatrix[0][j] = 1;
            gridMatrix[gridSize - 1][j] = 1;
        }
    }

    return true;
}

void Tema2::makeMazeObstacles() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float X = (gridSize - 1 - i) * enlargePlayground;
            float Z = j * enlargePlayground;
            if (gridMatrix[i][j] == 1) {
                obstacles.emplace_back(X, Z, enlargePlayground, 5, std::make_pair(i, j));
            }
        }
    }
}

void Tema2::spawnPlayer() {
    int i, j;
    do {
        i = rand() % (gridSize - 2) + 1;
        j = rand() % (gridSize - 2) + 1;
    } while(gridMatrix[i][j] != 0);

    gridMatrix[i][j] = 3;
    float X = (float) (gridSize - 1 - i) * enlargePlayground;
    float Z = (float) j * enlargePlayground;
    glm::vec3 humanTranslate = glm::vec3(0, 0.2f, 1.5f);
    glm::vec3 headTranslate = glm::vec3(0, 3, -3);
    player = Player2(X, Z, 1.125, 3.42, 0.5625, 2.5f, 0, 0.5f, gameCamera, humanTranslate, headTranslate, std::make_pair(i, j));
}

void Tema2::spawnEnemiesAndPickups() {
    int visited[gridSize][gridSize];
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            visited[i][j] = 0;
        }
    }

    std::set<std::pair<int, int>> currSet;
    currSet.insert(player.getGridPos());
    int stage = -1;
    numberOfSpacesToGoal = 1;
    bool gotToExit = false;
    while(!currSet.empty()) {
        std::set<std::pair<int, int>> nextSet;
        for (const std::pair<int, int> & pair : currSet) {
            int i = pair.first;
            int j = pair.second;
            visited[i][j] = 1;
            if (stage == 1) {
                // make pickup
                gridMatrix[i][j] = 4;
                float baseLength = 1.25f;
                float X = (float) (gridSize - 1 - i) * enlargePlayground + obstacles.back().getLengthOX() / 2 - baseLength / 2;
                float Z = (float) j * enlargePlayground + obstacles.back().getLengthOZ() / 2 - baseLength / 2;

                PickupType type = HEALTH;
                float timer = 0;
                int whichType = rand() % 4;
                switch (whichType) {
                    case 0:
                        type = HEALTH;
                        timer = 0;
                        break;

                    case 1:
                        type = STOPTIME;
                        timer = 3;
                        break;

                    case 2:
                        type = SLOWTIME;
                        timer = 6;
                        break;

                    case 3:
                        type = FIRERATEUP;
                        timer = 12;
                        break;
                }

                pickups.emplace_back(X, Z, baseLength, 2, type, timer, std::make_pair(i, j));
            } else if (stage == 2) {
                // make enemy
                gridMatrix[i][j] = 2;
                float enemyScale = 1;
                float X = (float) (gridSize - 1 - i) * enlargePlayground + enemyScale * 0.5f;
                float Z = (float) j * enlargePlayground + enemyScale * 2;
                enemies.emplace_back(X, enemyScale, Z, 1.125f * enemyScale, 1.5f * enemyScale, 0.5625f * enemyScale, 20, 0, pair, enemyScale, 2);
            }

            // North
            if (pair.first > 0 && gridMatrix[i - 1][j] == 0 && visited[i - 1][j] == 0) {
                nextSet.emplace(i - 1, j);
            }

            // South
            if (pair.first < gridSize - 1 && gridMatrix[i + 1][j] == 0 && visited[i + 1][j] == 0) {
                nextSet.emplace(i + 1, j);
            }

            // West
            if (pair.second > 0 && gridMatrix[i][j - 1] == 0 && visited[i][j - 1] == 0) {
                nextSet.emplace(i, j - 1);
            }

            // East
            if (pair.second < gridSize - 1 && gridMatrix[i][j + 1] == 0 && visited[i][j + 1] == 0) {
                nextSet.emplace(i, j + 1);
            }

            if (i == exitGridPos.first && j == exitGridPos.second) {
                gotToExit = true;
            }
        }

        currSet = nextSet;
        if (stage <= 1) {
            stage++;
        } else {
            stage = 0;
        }

        if (!gotToExit) {
            numberOfSpacesToGoal++;
        }
    }
}

void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // set the screen area of the game and draw
    glViewport(0, 0, resolution.x, resolution.y);
    DrawScene();

    DrawHUD();
}

void Tema2::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear) {
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


void Tema2::DrawHUD() {
    glm::ivec2 resolution = window->GetResolution();

    // draw health bar
    {
        float healthBarLengthOX = resolution.x / 18;
        float healthBarLengthOY = resolution.y / 3;

        // draw health bar outline
        ViewportSpace viewSpace = ViewportSpace(resolution.x / 1.4f, resolution.y / 18, healthBarLengthOX, healthBarLengthOY);
        SetViewportArea(viewSpace, glm::vec3(1), true);

        // draw health bar
        float healthBarHeight = player.getHealth() <= 1 ? healthBarLengthOY * player.getHealth() : 0;
        viewSpace = ViewportSpace(resolution.x / 1.4f, resolution.y / 18, healthBarLengthOX, healthBarHeight);
        SetViewportArea(viewSpace, glm::vec3(1, 0, 0), true);
    }

    // draw timer
    {
        float timerBarLengthOX = resolution.y / 2.25f;
        float timerBarLengthOY = resolution.x / 20;

        // draw timer bar outline
        ViewportSpace viewSpace = ViewportSpace(resolution.x / 1.4f, resolution.y / 2.35f, timerBarLengthOX, timerBarLengthOY);
        SetViewportArea(viewSpace, glm::vec3(1), true);

        // draw timer bar
        float timeLeft = timer / initialTime;
        if (timer < 0) {
            timeLeft = 0;
        }

        viewSpace = ViewportSpace(resolution.x / 1.4f, resolution.y / 2.35f, timerBarLengthOX * timeLeft, timerBarLengthOY);
        SetViewportArea(viewSpace, glm::vec3(0.196f, 0.804f, 0.196f), true);
    }

    // draw the minimap
    {
        ViewportSpace viewSpace = ViewportSpace(resolution.x / 1.27f, resolution.y / 18, resolution.x / 5.65f, resolution.y / 3);
        SetViewportArea(viewSpace, glm::vec3(0, 0, 1), true);
        DrawSceneMinimap();
        miniMapCamera->SetX(player.getX());
        miniMapCamera->SetZ(player.getZ());
    }
}

void Tema2::DrawGeneralScene() {
    // render map space
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(gridSize * enlargePlayground));
    RenderAlignedBox(shaders["ColorShader"], modelMatrix, glm::vec3(0.5f));

    // render maze walls
    for (Obstacle2 & obstacle : obstacles) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(obstacle.getX(), 0, obstacle.getZ()));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(obstacle.getLengthOX(), obstacle.getLengthOY(), obstacle.getLengthOX()));
        RenderAlignedBox(shaders["ColorShader"], modelMatrix, glm::vec3(0));
    }

    // render exit
    {
        // render exit door
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(exitPos.first, 0, exitPos.second));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(obstacles.back().getLengthOX(), obstacles.back().getLengthOY(), 0.1f));
        RenderAlignedBox(shaders["ColorShader"], modelMatrix, glm::vec3(0, 0, 1));

        // render exit sign
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(exitPos.first +  obstacles.back().getLengthOX() / 2 - 0.1f * enlargePlayground, obstacles.back().getLengthOY(), exitPos.second - exitSignOffsetZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f * enlargePlayground, 0.05f * enlargePlayground, 0.05f * enlargePlayground));
        RenderAlignedBox(shaders["ColorShader"], modelMatrix, glm::vec3(0, 0.7f, 0));
    }

    // render pickups
    for (auto pickup : pickups) {
        if (!pickup.isItVisible()) {
            continue;
        }

        glm::vec3 color = glm::vec3(0);
        switch (pickup.getType()) {
            case HEALTH:
                color = glm::vec3(0, 1, 0);
                break;

            case STOPTIME:
                color = glm::vec3(0.625f, 0.125f, 0.9375f);
                break;

            case SLOWTIME:
                color = glm::vec3(1, 0, 1);
                break;

            case FIRERATEUP:
                color = glm::vec3(1, 1, 0);
                break;
        }

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(pickup.getX(), pickup.getY(), pickup.getZ()));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(pickup.getLengthOX(), pickup.getLengthOY(), pickup.getLengthOZ()));
        RenderAlignedBox(shaders["ColorShader"], modelMatrix, color);
    }
}

void Tema2::DrawSceneMinimap() {
    this->camera = miniMapCamera;

    DrawGeneralScene();

    glm::mat4 startMatrix = glm::mat4(1);
    startMatrix = glm::translate(startMatrix, glm::vec3(player.getX(), 0, player.getZ()));
    startMatrix = glm::scale(startMatrix, glm::vec3(3));
    RenderMesh(meshes["box"], shaders["PlayerShader"], startMatrix, glm::vec3(1, 0.88f, 0.74f));

    for (Enemy2 & enemy : enemies) {
        if (enemy.isItDead()) {
            continue;
        }

        glm::mat4 startMatrix = glm::mat4(1);
        startMatrix = glm::translate(startMatrix, glm::vec3(enemy.getX(), enemy.getY(), enemy.getZ()));
        startMatrix = glm::translate(startMatrix, player.getHumanTranslate());
        startMatrix = glm::scale(startMatrix, glm::vec3(enemy.getScale()));
        RenderMesh(meshes["box"], shaders["PlayerShader"], startMatrix, glm::vec3(1, 0, 0));
    }
}

void Tema2::DrawScene() {
    this->camera = gameCamera;

    DrawGeneralScene();
    glm::mat4 modelMatrix;

    // render player
    if (!player.getAttackMode()) {
        glm::mat4 startMatrix = glm::mat4(1);
        startMatrix = glm::translate(startMatrix, glm::vec3(player.getX(), 0, player.getZ()));
        startMatrix = glm::scale(startMatrix, glm::vec3(0.5f));

        glm::vec3 skinColor = glm::vec3(1, 0.88f, 0.74f);
        glm::vec3 jeansColor = glm::vec3(0.08f, 0.38f, 0.74f);
        glm::vec3 camoColor = glm::vec3(0.28f, 0.3f, 0.21f);
        glm::vec3 armorPlateColor = glm::vec3(0.65f, 0.64f, 0.63f);

        // head
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= transform3D::Translate(0, 3, -3);
        modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        modelMatrix *= transform3D::RotateOY(player.getAngular());
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, skinColor);

        // used for rotating the Player along with the camera
        float scale_arm = 0.5f;
        glm::mat4 torso_matrix = glm::mat4(1);
        torso_matrix = glm::translate(torso_matrix, torsoTranslate);
        torso_matrix *= transform3D::RotateOY(player.getAngular());

        // torso
        float scale_torso = 0.75f;
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::Scale(1.5f * scale_torso, 2 * scale_torso, 0.75f * scale_torso);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, camoColor);

        // armor plate
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::Translate(0, 0, -0.3f);
        modelMatrix *= transform3D::Scale(1 * scale_torso, 1.5f * scale_torso, 0.01f);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, armorPlateColor);

        // left arm
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::Translate(0.72f, 0.5f, 0);
        modelMatrix *= transform3D::RotateOX(membersAngle);
        modelMatrix *= transform3D::Translate(0, -0.5f, 0);
        modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, skinColor);

        // right arm
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::Translate(-0.72f, 0.5f, 0);
        modelMatrix *= transform3D::RotateOX(-membersAngle);
        modelMatrix *= transform3D::Translate(0, -0.5f, 0);
        modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, skinColor);

        // right femur
        float scale_leg = 0.6f;
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::RotateOX(membersAngle);
        modelMatrix = glm::translate(modelMatrix, -torsoTranslate);
        modelMatrix *= transform3D::Translate(-0.28f, 0.82f, -3);
        modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, jeansColor);

        // left femur
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::RotateOX(-membersAngle);
        modelMatrix = glm::translate(modelMatrix, -torsoTranslate);
        modelMatrix *= transform3D::Translate(0.25f, 0.82f, -3);
        modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, jeansColor);

        // right tibia
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::RotateOX(membersAngle);
        modelMatrix = glm::translate(modelMatrix, -torsoTranslate);
        modelMatrix *= transform3D::Translate(-0.28f, 0.08f, -3);
        modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, skinColor);

        // left tibia
        modelMatrix = glm::mat4(1) * startMatrix;
        modelMatrix *= torso_matrix;
        modelMatrix *= transform3D::RotateOX(-membersAngle);
        modelMatrix = glm::translate(modelMatrix, -torsoTranslate);
        modelMatrix *= transform3D::Translate(0.25f, 0.08f, -3);
        modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
        RenderMesh(meshes["box"], shaders["PlayerShader"], modelMatrix, skinColor);
    }

    for (Enemy2 & enemy : enemies) {
        if (enemy.isItDead() && !enemy.isItCurrentlyDisplaced()) {
            continue;
        }

        currEnemyDisplacementTime = enemy.getDisplacementTimer();
        isCurrEnemyVertexDisplaced = enemy.isItCurrentlyDisplaced();
        // render enemy
        {
            glm::mat4 startMatrix = glm::mat4(1);
            startMatrix = glm::translate(startMatrix, glm::vec3(enemy.getX(), enemy.getY(), enemy.getZ()));
            startMatrix = glm::translate(startMatrix, player.getHumanTranslate());
            startMatrix = glm::scale(startMatrix, glm::vec3(enemy.getScale()));

            // head
            modelMatrix = glm::mat4(1) * startMatrix;
            modelMatrix *= transform3D::Translate(0, 1.5, -3);
            modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
            modelMatrix *= transform3D::RotateOY(enemy.getAngular());
            RenderMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));

            float scale_arm = 0.5f;
            glm::mat4 torso_matrix = glm::mat4(1);
            torso_matrix = glm::translate(torso_matrix, glm::vec3(0, 0.5, -3));
            torso_matrix *= transform3D::RotateOY(enemy.getAngular());

            // torso
            float scale_torso = 0.75f;
            modelMatrix = glm::mat4(1) * startMatrix;
            modelMatrix *= torso_matrix;
            modelMatrix *= transform3D::Scale(1.5f * scale_torso, 2 * scale_torso, 0.75f * scale_torso);
            RenderMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(0));

            // left arm
            modelMatrix = glm::mat4(1) * startMatrix;
            modelMatrix *= torso_matrix;
            modelMatrix *= transform3D::Translate(0.72f, 0.5f, 0);
            modelMatrix *= transform3D::RotateOX(membersEnemyAngle);
            modelMatrix *= transform3D::Translate(0, -0.5f, 0);
            modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
            RenderMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));

            // right arm
            modelMatrix = glm::mat4(1) * startMatrix;
            modelMatrix *= torso_matrix;
            modelMatrix *= transform3D::Translate(-0.72f, 0.5f, 0);
            modelMatrix *= transform3D::RotateOX(-membersEnemyAngle);
            modelMatrix *= transform3D::Translate(0, -0.5f, 0);
            modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
            RenderMesh(meshes["box"], shaders["EnemyShader"], modelMatrix, glm::vec3(1, 0, 0));
        }
    }

    // render projectiles
    for (auto projctl : visibleProjectiles) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(projctl.getX(), projctl.getY(), projctl.getZ()));;
        float projctlScale = projctl.isItVisible() ? 1 : 0;
        modelMatrix = glm::scale(modelMatrix, glm::vec3(projctlScale * projctl.getLengthOX()));
        RenderMesh(meshes["sphere"], shaders["ColorShader"], modelMatrix, glm::vec3(1, 0, 0));
    }
}

void Tema2::Update(float deltaTimeSeconds) {
    if (!player.isItDead()) {
        if (!player.didTheTimeStop()) {
            membersEnemyAngle += 11 * deltaTimeSeconds;
        }

        // do stuff with enemies
        for (Enemy2 &enemy: enemies) {
            enemy.passTime(deltaTimeSeconds);
            if (enemy.isItDead() || player.didTheTimeStop()) {
                continue;
            }

            enemy.move(deltaTimeSeconds, pi);
            enemy.checkForNextStage(obstacles.back(), deltaTimeSeconds);

            if (player.didTheTimeSlow()) {
                enemy.slowDown();
            } else {
                enemy.speedBack();
            }
        }

        // do stuff with visible projectiles
        {
            int i = 0, size = visibleProjectiles.size();
            std::list<Projectile2> remainingVisibleProjectiles;
            while (i < size && size > 0) {
                Projectile2 projctl = visibleProjectiles.back();
                visibleProjectiles.pop_back();

                if (!projctl.isItVisible()) {
                    invisibleProjectiles.push_back(projctl);
                    i++;
                    continue;
                }

                projctl.moveTowardsTarget(deltaTimeSeconds);

                for (Enemy2 & enemy : enemies) {
                    if (enemy.isItDead()) {
                        continue;
                    }

                    if (projctl.checkCollisionEnemy(&enemy)) {
                        enemy.die();
                        projctl.disappear();
                    }
                }

                float mapLength = gridSize * enlargePlayground;
                projctl.collisionWithOutsideMap(0, 0, 0, mapLength, mapLength, mapLength);
                projctl.collisionWithObstacles(obstacles);
                projctl.collisionWithPickups(pickups);

                if (!projctl.isItVisible()) {
                    invisibleProjectiles.push_back(projctl);
                } else {
                    remainingVisibleProjectiles.push_back(projctl);
                }

                i++;
            }

            visibleProjectiles.clear();
            visibleProjectiles = remainingVisibleProjectiles;
        }

        player.collisionWithEnemies(enemies, deltaTimeSeconds);
        for (Pickup2 & pickup : pickups) {
            if (pickup.isItVisible()) {
                if (player.collisionWithPickup(&pickup)) {
                    pickup.disappear();
                }
            }
        }

        player.passTime(deltaTimeSeconds);

        float timePassed = player.didTheTimeSlow() ? deltaTimeSeconds / 3 : deltaTimeSeconds;
        if (!player.didTheTimeStop()) {
            timer -= timePassed;
        }

        if (timer < 0) {
            printf("The time has run out! YOU LOST! Press ENTER to start another game!\n");
            player.die();
        }

        if (player.getHealth() <= 0) {
            printf("You were killed! YOU LOST! Press ENTER to start another game!\n");
            player.die();
        }

        if (player.checkCollisionWithOutsideMap(0, gridSize * enlargePlayground)) {
            if (timer > initialTime / 2) {
                printf("You are a TRUE PLAYER!");
            } else if (timer > initialTime * 9 / 10) {
                printf("HOW did YOU even manage THAT time?!?!?");
            } else {
                printf("Congratulations, you got out of the labyrinth!");
            }

            printf(" YOU WON! Press ENTER to start another game!\n");
            player.die();
        }
    }
}


void Tema2::FrameEnd() {
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 &color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;
    
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));
    if (!strcmp(shader->GetName(), "EnemyShader")) {
        glUniform1i(glGetUniformLocation(shader->program, "shouldBeDisplaced"), isCurrEnemyVertexDisplaced);
        glUniform1f(glGetUniformLocation(shader->program, "displacementTime"), currEnemyDisplacementTime);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderAlignedBox(Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 &color) {
    glm::mat4 matrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    RenderMesh(meshes["box"], shader, matrix, color);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (!player.isItDead()) {
        float distance = deltaTime;
        if (window->KeyHold(GLFW_KEY_W)) {
            player.moveForward(distance);
            membersAngle += membersAngleSign * deltaTime;
            if (membersAngle > pi / 8 || membersAngle < -pi / 8) {
                membersAngleSign *= -1;
                membersAngle += membersAngleSign * deltaTime;
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            player.moveLeft(distance);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            player.moveBackwards(distance);
            membersAngle -= membersAngleSign * deltaTime;
            if (membersAngle > pi / 8 || membersAngle < -pi / 8) {
                membersAngleSign *= -1;
                membersAngle -= membersAngleSign * deltaTime;
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            player.moveRight(distance);
        }

        bool W = window->KeyHold(GLFW_KEY_W);
        bool A = window->KeyHold(GLFW_KEY_A);
        bool S = window->KeyHold(GLFW_KEY_S);
        bool D = window->KeyHold(GLFW_KEY_D);
        player.collisionWithObstacles(obstacles, deltaTime, W, A, S, D);

        if (window->KeyHold(GLFW_KEY_Q)) {
            miniMapCamera->AddPosition(glm::vec3(0, -deltaTime * gridSize * 2, 0));
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            miniMapCamera->AddPosition(glm::vec3(0, deltaTime * gridSize * 2, 0));
        }
    }
}


void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_ENTER && player.isItDead()) {
        resetGame();
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (!player.isItDead()) {
        player.setAngular(player.getAngular() - sensivityOY * (float) deltaX);
        if (player.getAttackMode()) {
            gameCamera->RotateFirstPerson_OX(-sensivityOX * (float) deltaY);
            gameCamera->RotateFirstPerson_OY(-sensivityOY * (float) deltaX);
        }

        if (!player.getAttackMode()) {
            gameCamera->RotateThirdPerson_OY(-sensivityOY * (float) deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !player.isItDead()) {
        player.changeMode();
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && player.canItFire() && player.getAttackMode() && !player.isItDead()) {
        Projectile2 projctl;
        if (invisibleProjectiles.empty()) {
            projctl = Projectile2(1, 30, 0, enlargePlayground * 2);
        } else {
            projctl = invisibleProjectiles.back();
            invisibleProjectiles.pop_back();
        }

        projctl.launchProjectile(gameCamera->GetForwardVector(), glm::vec3(player.getCameraCenterX(), 1, player.getCameraCenterZ()));
        visibleProjectiles.push_back(projctl);
        player.disableFire();
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}


void Tema2::OnWindowResize(int width, int height)
{
}
