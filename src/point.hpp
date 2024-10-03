#pragma once

#include <glm/glm.hpp>

struct Point {
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(1);
    
    Point(glm::vec3 position = glm::vec3(0), glm::vec4 color = glm::vec4(1));
    glm::mat4 mat4();
};

