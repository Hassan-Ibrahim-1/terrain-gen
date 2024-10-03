#include "rect.hpp"
#include "transform.hpp"

Rect::Rect(Transform transform, glm::vec4 color)
    : transform(transform), color(color) {}

Rect::Rect(glm::vec3 position, glm::vec4 color)
    : transform(position),
      color(color) {}

Rect::Rect(
    float x,
    float y,
    float z,
    float r,
    float g,
    float b,
    float a
) : Rect(glm::vec3(x, y, z), glm::vec4(r, g, b, a)) {}

