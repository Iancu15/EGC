#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(translateX, translateY, 1));

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::mat3(glm::vec3(scaleX, 0, 0), glm::vec3(0, scaleY, 0), glm::vec3(0, 0, 1));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::mat3(glm::vec3(glm::cos(radians), glm::sin(radians), 0), glm::vec3(-glm::sin(radians), glm::cos(radians), 0), glm::vec3(0, 0, 1));

    }
}   // namespace transform2D