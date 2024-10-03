#include <glm/gtc/matrix_transform.hpp>

#include "point.hpp"

Point::Point(glm::vec3 position, glm::vec4 color)
    : position(position), color(color) {}

glm::mat4 Point::mat4() {
    glm::mat4 mat(1);
    return glm::translate(mat, position);
}

