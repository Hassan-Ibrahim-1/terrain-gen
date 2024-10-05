#include "vertex.hpp"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal,
               glm::vec2 tex_coords, glm::vec3 color)
    : position(position), normal(normal), tex_coords(tex_coords), color(color) {}

