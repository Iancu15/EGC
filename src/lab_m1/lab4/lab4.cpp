#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}

void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Initialize human factors
    angular_torso_OY = 0;
    angular_tibia_OX = 0;
    translate_human_X = 0;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // head
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(translate_human_X, 3, -3);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // right femur
    float scale_leg = 0.6f;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-0.28f + translate_human_X, 0.92f, -3);
    modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // left femur
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.25f + translate_human_X, 0.92f, -3);
    modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    glm::mat4 knee_matrix = glm::mat4(1);
    knee_matrix *= transform3D::RotateOX(angular_tibia_OX);
    knee_matrix *= transform3D::Translate(translate_human_X, -0.37f, 0);

    // right tibia
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-0.28f, 0.57f, -3);
    modelMatrix *= knee_matrix;
    modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // left tibia
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.25f, 0.57f, -3);
    modelMatrix *= knee_matrix;
    modelMatrix *= transform3D::Scale(0.9f * scale_leg, 1.25f * scale_leg, 0.75f * scale_leg);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // torso
    float scale_arm = 0.5f;
    glm::mat4 torso_matrix =glm::mat4(1);
    torso_matrix *= transform3D::Translate(translate_human_X, 2, -3);
    torso_matrix *= transform3D::RotateOY(angular_torso_OY);

    float scale_torso = 0.75f;
    modelMatrix = glm::mat4(1);
    modelMatrix *= torso_matrix;
    modelMatrix *= transform3D::Scale(1.5f * scale_torso, 2 * scale_torso, 0.75f * scale_torso);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // left arm
    modelMatrix = glm::mat4(1);
    modelMatrix *= torso_matrix;
    modelMatrix *= transform3D::Translate(0.72f, 0, 0);
    modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // right arm
    modelMatrix = glm::mat4(1);
    modelMatrix *= torso_matrix;
    modelMatrix *= transform3D::Translate(-0.72f, 0, 0);
    modelMatrix *= transform3D::Scale(0.7f * scale_arm, 2.5f * scale_arm, 0.75f * scale_arm);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    float dist = 0.5 * deltaTime;
    if (window->KeyHold(GLFW_KEY_W)) {
        translateZ -= dist;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateZ += dist;
    }

    if (window->KeyHold(GLFW_KEY_R)) {
        translateY += dist;
    }

    if (window->KeyHold(GLFW_KEY_F)) {
        translateY -= dist;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= dist;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += dist;
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += dist;
        scaleY += dist;
        scaleZ += dist;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= dist;
        scaleY -= dist;
        scaleZ -= dist;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += dist;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= dist;
    }

    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += dist;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= dist;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += dist;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= dist;
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        angular_torso_OY += dist;
    }

    if (window->KeyHold(GLFW_KEY_I)) {
        angular_tibia_OX += dist;
    }

    if (window->KeyHold(GLFW_KEY_O)) {
        angular_tibia_OX -= dist;
    }

    if (window->KeyHold(GLFW_KEY_P)) {
        translate_human_X += dist;
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
