#pragma once

#include <glm/glm.hpp>
#include "transform.hpp"

struct Rect {
    Transform transform = Transform();
    glm::vec4 color = glm::vec4();

    Rect(Transform transform, glm::vec4 color);
    Rect(glm::vec3 position, glm::vec4 color);
    Rect(
        float x,
        float y,
        float z,
        float r,
        float g,
        float b,
        float a = 1.0f
    );
    Rect() = default;
};

