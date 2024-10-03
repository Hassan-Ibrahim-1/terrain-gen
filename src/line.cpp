#include "line.hpp"

Line::Line(Point p1, Point p2)
    : p1(p1), p2(p2) {}

void Line::set_color(glm::vec3 color) {
    p1.color = glm::vec4(color, 1);
    p2.color = glm::vec4(color, 1);
}

