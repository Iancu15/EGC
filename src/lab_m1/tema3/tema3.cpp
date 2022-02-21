//
// Created by alex on 21.12.2021.
//

#include "tema3.h"
#include "lab_m1/lab4/transform3D.h"
#include "cmath"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3() {}


Tema3::~Tema3() {}

void change_vec_color_randomly(glm::vec3 & color) {
    color.r = (float) (rand() % 256 + 1) / 255;
    color.g = (float) (rand() % 256 + 1) / 255;
    color.b = (float) (rand() % 256 + 1) / 255;
}

void Tema3::generateColors() {
    for (int i = 0; i < ringSize; i++) {
        for (int j = 0; j < ringSize; j++) {
            change_vec_color_randomly(ringPlanesColors[i][j]);
            planeColorsTransitionTime[i][j] = 0;
        }
    }

    for (int i = 0; i < numOfSpotlights; i++) {
        change_vec_color_randomly(spotlightsColors[i]);
        spotlightsTransitionTime[i] = 0;
    }
}

void Tema3::pickSpotlightPositions() {
    int step = 4;
    spotlightsPositions[0] = glm::vec3(ringSize/step, ringSize/2, ringSize/step);
    spotlightsPositions[1] = glm::vec3((step - 1) * ringSize/step, ringSize/2, ringSize/step);
    spotlightsPositions[2] = glm::vec3(ringSize/step, ringSize/2, (step - 1) * ringSize/step);
    spotlightsPositions[3] = glm::vec3((step - 1) * ringSize/step, ringSize/2, (step - 1) * ringSize/step);
}

void Tema3::pickDancersProperties() {
    float Y = 0.005f;
    initialDancersPositions[0] = glm::vec3(3, Y, 2);
    initialDancersPositions[1] = glm::vec3(5, Y, 2);
    initialDancersPositions[2] = glm::vec3(3, Y, 4);
    initialDancersPositions[3] = glm::vec3(5, Y, 4);
    initialDancersPositions[4] = glm::vec3(3, Y, 6);
    initialDancersPositions[5] = glm::vec3(5, Y, 6);
    for (int i = 0; i < numOfDancers; i++) {
        dancersPositions[i] = initialDancersPositions[i];
    }

    dancersRotationAngles[0] = glm::radians(90.f);
    dancersRotationAngles[1] = glm::radians(-90.f);
    dancersRotationAngles[2] = glm::radians(90.f);
    dancersRotationAngles[3] = glm::radians(-90.f);
    dancersRotationAngles[4] = glm::radians(90.f);
    dancersRotationAngles[5] = glm::radians(-90.f);
}

void printVec3(glm::vec3 vector) {
    printf("%f %f %f\n", vector.x, vector.y, vector.z);
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];
    for (int i = 0; i < size; i++) {
        data[i] = (unsigned char) (rand() % 256);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::Init() {
    mapTextures["random"] = CreateRandomTexture(16, 16);
    mapTextures["random_next"] = CreateRandomTexture(16, 16);

    {
        Mesh *mesh = new Mesh("archer");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters/archer"), "Archer.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create general shader used for most scene objects
    {
        Shader *shader = new Shader("GeneralShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create cone shader used for spotlights's cones
    {
        Shader *shader = new Shader("ConeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "ConeVertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "ConeFragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    // plane lights properties
    {
        planeLightsIntensity = 5;
        planeColorsTransitionFullTime = 1;
        planeLightsTransitionsPerSecond = 8;
        timeUntilNextPlannedTransitionPlanes = 1.0f / planeLightsTransitionsPerSecond;
        ringFloorSwitch = true;
    }

    // spotlights properties
    {
        spotlightsIntensity = 3;
        spotlightsTransitionFullTime = 1;
        spotlightsTransitionsPerSecond = 1;
        timeUntilNextPlannedTransitionSpotlights = 1.0f / spotlightsTransitionsPerSecond;
        rotateFrameFactor = 0.1f;
        spotlightsRotationAnglesOXLimit = 0.3f;
        minTimeBetweenReverses = 1;
        for (int i = 0; i < numOfSpotlights; i++) {
            spotlightsDirections[i] = glm::vec3(0, -1, 0);
            spotlightsRotationAnglesOX[i] = 0;
            spotlightsRotationAnglesOZ[i] = 0;
            additionalOZFrameFactor[i] = 0;
            timeSinceReverse[i] = 0;
        }

        spotlightsAngle = 30.f;
        spotlightsSwitch = true;
    }

    // disco globe light properties
    {
        discoGlobeScale = 1;
        discoGlobeLightPosition = glm::vec3(ringSize/2, (float) ringSize/2 - 0.5f * discoGlobeScale, ringSize/2);
        discoGlobeIntensity = 1;
        discolightTransitionFullTime = 1;
        discolightTransitionTime = 0;
        textureInterpolationFactor = 0;
        discoRotationFactor = 0;
        discoGlobeSwitch = true;
    }

    // create cone mesh
    {
        vector<VertexFormat> vertices;
        float step = 0.025f;
        float x = -0.5f;
        while (x < 0.5f) {
            float z = sqrt(0.5f * 0.5f - x * x);
            vertices.emplace_back(glm::vec3(x, -1, z), glm::vec3(0));
            x += step;
        }

        x = 0.5f;
        while (x > -0.5f) {
            float z = sqrt(0.5f * 0.5f - x * x);
            vertices.emplace_back(glm::vec3(x, -1, -z), glm::vec3(0));
            x -= step;
        }

        vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0));

        size_t numberOfVertices = vertices.size();
        vector<unsigned int> indices;
        for (size_t i = 0; i < numberOfVertices - 2; i++) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(numberOfVertices - 1);
        }

        // fragment between first and last vertices
        indices.push_back(numberOfVertices - 2);
        indices.push_back(0);
        indices.push_back(numberOfVertices - 1);

        meshes["cone"] = new Mesh("generated cone");
        meshes["cone"]->InitFromData(vertices, indices);
    }

    wallThickness = 0.003f;
    generateColors();
    pickSpotlightPositions();
    pickDancersProperties();
}


void Tema3::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::transitionLights(float deltaTimeSeconds) {
    // transition ring plane lights
    {
        if (timeUntilNextPlannedTransitionPlanes < 0) {
            int randI = rand() % ringSize;
            int randJ = rand() % ringSize;
            planeColorsTransitionTime[randI][randJ] = planeColorsTransitionFullTime;
            oldPlaneColors[randI][randJ] = ringPlanesColors[randI][randJ];
            change_vec_color_randomly(newPlaneColors[randI][randJ]);
            timeUntilNextPlannedTransitionPlanes = planeColorsTransitionFullTime / planeLightsTransitionsPerSecond;
        }

        for (int i = 0; i < ringSize; i++) {
            for (int j = 0; j < ringSize; j++) {
                if (planeColorsTransitionTime[i][j] > 0) {
                    float interpolation_factor = 1.0f - planeColorsTransitionTime[i][j] / planeColorsTransitionFullTime;
                    ringPlanesColors[i][j] = glm::mix(oldPlaneColors[i][j], newPlaneColors[i][j], interpolation_factor);
                    planeColorsTransitionTime[i][j] -= deltaTimeSeconds;
                    if (planeColorsTransitionTime[i][j] == 0) {
                        planeColorsTransitionTime[i][j] -= 0.1f;
                    }
                }

                if (planeColorsTransitionTime[i][j] < 0) {
                    planeColorsTransitionTime[i][j] = 0;
                    ringPlanesColors[i][j] = newPlaneColors[i][j];
                }
            }
        }

        timeUntilNextPlannedTransitionPlanes -= deltaTimeSeconds;
    }

    // transition spotlights
    {
        if (timeUntilNextPlannedTransitionSpotlights < 0) {
            int randI = rand() % numOfSpotlights;
            spotlightsTransitionTime[randI] = spotlightsTransitionFullTime;
            oldSpotlightColors[randI] = spotlightsColors[randI];
            change_vec_color_randomly(newSpotlightColors[randI]);
            timeUntilNextPlannedTransitionSpotlights = spotlightsTransitionFullTime / spotlightsTransitionsPerSecond;
        }

        for (int i = 0; i < numOfSpotlights; i++) {
            if (spotlightsTransitionTime[i] > 0) {
                float interpolation_factor = 1.0f - spotlightsTransitionTime[i] / spotlightsTransitionFullTime;
                spotlightsColors[i] = glm::mix(oldSpotlightColors[i], newSpotlightColors[i], interpolation_factor);
                spotlightsTransitionTime[i] -= deltaTimeSeconds;
                if (spotlightsTransitionTime[i] == 0) {
                    spotlightsTransitionTime[i] -= 0.1f;
                }
            }

            if (spotlightsTransitionTime[i] < 0) {
                spotlightsTransitionTime[i] = 0;
                spotlightsColors[i] = newSpotlightColors[i];
            }
        }

        timeUntilNextPlannedTransitionSpotlights -= deltaTimeSeconds;
    }

    // transition disco light texture
    {
        int rand_number = rand() % (int) (1.0f / deltaTimeSeconds);
        if (rand_number == 0 && discolightTransitionTime == 0) {
            mapTextures["random_next"] = CreateRandomTexture(16, 16);
            discolightTransitionTime = discolightTransitionFullTime;
        }

        if (discolightTransitionTime > 0) {
            textureInterpolationFactor = 1.0f - discolightTransitionTime / discolightTransitionFullTime;
            discolightTransitionTime -= deltaTimeSeconds;
            if (discolightTransitionTime == 0) {
                discolightTransitionTime -= 0.1f;
            }
        }

        if (discolightTransitionTime < 0) {
            discolightTransitionTime = 0;
            mapTextures["random"] = mapTextures["random_next"];
            textureInterpolationFactor = 0;
        }
    }
}

glm::vec3 getPlaneLightSourcePos(int i, int j) {
    float X = (float) ringSize - 1 - (float) i;
    float Z = (float) j;
    return {X + 0.5f, 0, Z + 0.5f};
}

void Tema3::rotateSpotlights(float deltaTimeSeconds) {
    for (int i = 0; i < numOfSpotlights; i++) {
        timeSinceReverse[i] += deltaTimeSeconds;
        float frameAngleOX = deltaTimeSeconds * rotateFrameFactor;
        float frameAngleOZ = frameAngleOX * additionalOZFrameFactor[i];
        if (frameAngleOX >= spotlightsRotationAnglesOX[i] && spotlightsRotationAnglesOX[i] >= 0) {
            additionalOZFrameFactor[i] = (float) (rand() % 20) / 10;
        }

        spotlightsDirections[i] = glm::vec3(glm::vec4(spotlightsDirections[i], 1) * transform3D::RotateOX(frameAngleOX));
        spotlightsDirections[i] = glm::vec3(glm::vec4(spotlightsDirections[i], 1) * transform3D::RotateOZ(frameAngleOZ));
        spotlightsRotationAnglesOX[i] -= frameAngleOX;
        spotlightsRotationAnglesOZ[i] -= frameAngleOZ;
        if (abs(spotlightsRotationAnglesOX[i]) > spotlightsRotationAnglesOXLimit && timeSinceReverse[i] > minTimeBetweenReverses) {
            rotateFrameFactor *= -1;
            timeSinceReverse[i] = 0;
        }
    }
}

void Tema3::moveDancers(float seconds) {
    for (int i = 0; i < numOfDancers; i++) {
        float partnerDirection = i % 2 ? 1 : -1;
        dancersRotationAngles[i] += seconds * partnerDirection;
    }
}

void Tema3::Update(float deltaTimeSeconds) {
    transitionLights(deltaTimeSeconds);
    rotateSpotlights(deltaTimeSeconds);
    moveDancers(deltaTimeSeconds);
    discoRotationFactor += deltaTimeSeconds / 4;

    // Render dance ring
    for (int i = 0; i < ringSize; i++) {
        for (int j = 0; j < ringSize; j++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::vec3 pos = getPlaneLightSourcePos(i, j);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, wallThickness, 1));
            glm::vec3 color = ringFloorSwitch ? ringPlanesColors[i][j] : glm::vec3(0);
            RenderSimpleMesh(meshes["box"], shaders["GeneralShader"], modelMatrix, pos, color, 1);
        }
    }

    // Render walls
    {
        // render left wall
        for (int i = 0; i < ringSize; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::vec3 pos = glm::vec3((float) i + 0.5f, 2, 0);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix *= transform3D::RotateOX(M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, wallThickness, 4));
            RenderSimpleMesh(meshes["box"], shaders["GeneralShader"], modelMatrix, pos);
        }

        // render back wall
        for (int i = 0; i < ringSize; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::vec3 pos = glm::vec3(ringSize, 2, (float) i + 0.5f);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix *= transform3D::RotateOZ(M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(4, wallThickness, 1));
            RenderSimpleMesh(meshes["box"], shaders["GeneralShader"], modelMatrix, pos);
        }

        // render right wall
        for (int i = 0; i < ringSize; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::vec3 pos = glm::vec3((float) i + 0.5f, 2, ringSize);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix *= transform3D::RotateOX(M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, wallThickness, 4));
            RenderSimpleMesh(meshes["box"], shaders["GeneralShader"], modelMatrix, pos);
        }
    }

    // render ceiling
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos = glm::vec3(ringSize / 2, ringSize / 2, ringSize / 2);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix *= transform3D::RotateOY(M_PI / 2);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(8, wallThickness, 8));
        RenderSimpleMesh(meshes["box"], shaders["GeneralShader"], modelMatrix, pos);
    }

    // render disco globe
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos = discoGlobeLightPosition;
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(discoGlobeScale));
        RenderSimpleMesh(meshes["sphere"], shaders["GeneralShader"], modelMatrix, pos, glm::vec3(0), 1, 1);
    }

    // render dancers
    for (int i = 0; i < numOfDancers; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos = initialDancersPositions[i];
        float centreOffset = (i % 2) ? 1 : -1;
        modelMatrix = glm::translate(modelMatrix, pos + glm::vec3(centreOffset, 0, 0));
        modelMatrix *= transform3D::RotateOY(dancersRotationAngles[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-centreOffset, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
        dancersPositions[i] = glm::vec3(modelMatrix * glm::vec4(0, 0, 0, 1));
        RenderSimpleMesh(meshes["archer"], shaders["GeneralShader"], modelMatrix, dancersPositions[i], glm::vec3(0));
    }
}


void Tema3::FrameEnd() {
    DrawCoordinateSystem();
    glEnable (GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw spotlights's cones
    for (int i = 0; i < numOfSpotlights; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 pos = spotlightsPositions[i];
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix *= transform3D::RotateOX(spotlightsRotationAnglesOX[i]);
        modelMatrix *= transform3D::RotateOZ(spotlightsRotationAnglesOZ[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(glm::radians(spotlightsAngle)), 1, tan(glm::radians(spotlightsAngle))) * glm::vec3((float)ringSize));
        RenderSimpleMesh(meshes["cone"], shaders["ConeShader"], modelMatrix, pos);
    }

    glDepthMask(GL_TRUE);
    glDisable (GL_BLEND);
    glDisable (GL_CULL_FACE);
}

std::pair<int, int> Tema3::getClosestPlaneLightSource(glm::vec3 pos) {
    float minDist = ringSize;
    int minI = 0;
    int minJ = 0;
    for (int i = 0; i < ringSize; i++) {
        for (int j = 0; j < ringSize; j++) {
            glm::vec3 lightSourcePos = getPlaneLightSourcePos(i, j);
            float dist = distance(pos, lightSourcePos);
            if (dist < minDist) {
                minDist = dist;
                minI = i;
                minJ = j;
            }
        }
    }

    return std::make_pair(minI, minJ);
}

bool isIndexInGrid(int index) {
    if (index >= 0 && index < ringSize) {
        return true;
    }

    return false;
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 pos,
                             const glm::vec3 &color, float materialKe, int applyTexture) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    glm::vec3 lightPositionVec[9];
    glm::vec3 lightColorVec[9];
    std::pair<int, int> closestLightSourceGridPos = getClosestPlaneLightSource(pos + glm::vec3(0, 0, 0.5f));
    int i = closestLightSourceGridPos.first;
    int j = closestLightSourceGridPos.second;
    int numberOfPlaneLights = 0;
    for (int iOffset = -1; iOffset < 2; iOffset++) {
        for (int jOffset = -1; jOffset < 2; jOffset++) {
            int currI = i + iOffset;
            int currJ = j + jOffset;
            if (isIndexInGrid(currI) && isIndexInGrid(currJ)) {
                int vecIndex = numberOfPlaneLights;
                lightPositionVec[vecIndex] = getPlaneLightSourcePos(currI, currJ);
                lightColorVec[vecIndex] = ringPlanesColors[currI][currJ];
                numberOfPlaneLights++;
            }
        }
    }

    // send plane lights info
    {
        glUniform3fv(glGetUniformLocation(shader->program, "plane_lights_positions"), 9, glm::value_ptr(lightPositionVec[0]));
        glUniform3fv(glGetUniformLocation(shader->program, "plane_lights_colors"), 9, glm::value_ptr(lightColorVec[0]));
        glUniform1i(glGetUniformLocation(shader->program, "number_of_plane_lights"), numberOfPlaneLights);
        glUniform1f(glGetUniformLocation(shader->program, "plane_lights_intensity"), planeLightsIntensity);
        glUniform1i(glGetUniformLocation(shader->program, "ring_floor_switch"), ringFloorSwitch);
    }

    // send spotlights info
    {
        glUniform3fv(glGetUniformLocation(shader->program, "spotlights_positions"), numOfSpotlights, glm::value_ptr(spotlightsPositions[0]));
        glUniform3fv(glGetUniformLocation(shader->program, "spotlights_colors"), numOfSpotlights, glm::value_ptr(spotlightsColors[0]));
        glUniform3fv(glGetUniformLocation(shader->program, "spotlights_directions"), numOfSpotlights, glm::value_ptr(spotlightsDirections[0]));
        glUniform1f(glGetUniformLocation(shader->program, "spotlights_intensity"), spotlightsIntensity);
        glUniform1f(glGetUniformLocation(shader->program, "spot_angle"), spotlightsAngle);
        glUniform1i(glGetUniformLocation(shader->program, "spotlights_switch"), spotlightsSwitch);
    }

    // send disco globe light info
    {
        glUniform3fv(glGetUniformLocation(shader->program, "disco_ball_light_position"), 1, glm::value_ptr(discoGlobeLightPosition));
        glUniform1f(glGetUniformLocation(shader->program, "disco_ball_light_intensity"), discoGlobeIntensity);
        glUniform1f(glGetUniformLocation(shader->program, "texture_interpolation_factor"), textureInterpolationFactor);
        glUniform1i(glGetUniformLocation(shader->program, "apply_texture"), applyTexture);
        glUniform1f(glGetUniformLocation(shader->program, "disco_rotation_factor"), discoRotationFactor);
        glUniform1i(glGetUniformLocation(shader->program, "disco_globe_switch"), discoGlobeSwitch);
    }

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color)
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    int object_position = glGetUniformLocation(shader->program, "object_position");
    glUniform3f(object_position, pos.x, pos.y, pos.z);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapTextures["random"]->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_curr"), 0);
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mapTextures["random_next"]->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_next"), 1);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods) {
}


void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_Z) {
        ringFloorSwitch = !ringFloorSwitch;
    }

    if (key == GLFW_KEY_X) {
        spotlightsSwitch = !spotlightsSwitch;
    }

    if (key == GLFW_KEY_C) {
        discoGlobeSwitch = !discoGlobeSwitch;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}