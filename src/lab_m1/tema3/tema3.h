//
// Created by alex on 21.12.2021.
//

#ifndef GFXFRAMEWORK_TEMA3_H
#define GFXFRAMEWORK_TEMA3_H

#include "components/simple_scene.h"
#include "components/transform.h"

#define ringSize 8
#define numOfSpotlights 4
#define numOfDancers 6

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 pos,
                              const glm::vec3 &color = glm::vec3(0), float materialKe = 0, int applyTexture = 0);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        int materialShininess;
        float materialKd;
        float materialKs;
        float wallThickness;

        // ring planes variables
        glm::vec3 ringPlanesColors[ringSize][ringSize];
        float planeColorsTransitionTime[ringSize][ringSize];
        float planeColorsTransitionFullTime;
        glm::vec3 newPlaneColors[ringSize][ringSize];
        glm::vec3 oldPlaneColors[ringSize][ringSize];
        float planeLightsIntensity;
        float timeUntilNextPlannedTransitionPlanes;
        float planeLightsTransitionsPerSecond;
        bool ringFloorSwitch;

        // spotlights variables
        glm::vec3 spotlightsPositions[numOfSpotlights];
        glm::vec3 spotlightsColors[numOfSpotlights];
        glm::vec3 spotlightsDirections[numOfSpotlights];
        float spotlightsTransitionTime[numOfSpotlights];
        float spotlightsTransitionFullTime;
        glm::vec3 newSpotlightColors[numOfSpotlights];
        glm::vec3 oldSpotlightColors[numOfSpotlights];
        float spotlightsIntensity;
        float timeUntilNextPlannedTransitionSpotlights;
        float spotlightsTransitionsPerSecond;
        float spotlightsAngle;
        float spotlightsRotationAnglesOX[numOfSpotlights];
        float spotlightsRotationAnglesOZ[numOfSpotlights];
        float rotateFrameFactor;
        float additionalOZFrameFactor[numOfSpotlights];
        float spotlightsRotationAnglesOXLimit;
        bool spotlightsSwitch;
        float minTimeBetweenReverses;
        float timeSinceReverse[numOfSpotlights];

        // disco globe variables
        glm::vec3 discoGlobeLightPosition;
        float discoGlobeScale;
        float discoGlobeIntensity;
        float discolightTransitionFullTime;
        float discolightTransitionTime;
        float textureInterpolationFactor;
        float discoRotationFactor;
        bool discoGlobeSwitch;

        // dancers variables
        glm::vec3 dancersPositions[numOfDancers];
        glm::vec3 initialDancersPositions[numOfDancers];
        float dancersRotationAngles[numOfDancers];

        std::unordered_map<std::string, Texture2D *> mapTextures;

        void generateColors();

        std::pair<int, int> getClosestPlaneLightSource(glm::vec3 pos);

        void transitionLights(float deltaTimeSeconds);

        void pickSpotlightPositions();

        Texture2D *CreateRandomTexture(unsigned int width, unsigned int height);

        void rotateSpotlights(float deltaTimeSeconds);

        void pickDancersProperties();

        void moveDancers(float seconds);
    };
}   // namespace m1


#endif //GFXFRAMEWORK_TEMA3_H
