#pragma once

#include <glm/glm.hpp>

#include "point.hpp"

struct Line {
    Point p1;
    Point p2;

    Line(Point p1, Point p2);

    void set_color(glm::vec3 color = glm::vec3(1));
};

