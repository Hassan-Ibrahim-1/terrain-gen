#pragma once

#include "transform.hpp"

struct Circle {
    Transform transform = Transform(); // transform.position is the center point
    glm::vec4 color = glm::vec4(1);

    Circle(Transform transform = Transform(), glm::vec4 color = glm::vec4(1));
    Circle(glm::vec3 position, glm::vec4 color = glm::vec4(1));

    float radius();
    void render();
    
    friend bool operator==(const Circle& c1, const Circle& c2);
    friend bool operator!=(const Circle& c1, const Circle& c2);
};

