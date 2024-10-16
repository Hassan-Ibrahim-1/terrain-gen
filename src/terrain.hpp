#pragma once

#include "mesh.hpp"

class Terrain {
public:
    Terrain();

    void generate(uint nvertices, const Rect& bounds);
    void render();

private:
    Mesh _mesh;

    static constexpr float _color_boundary = 0.3f;
    static constexpr glm::vec3 _color_low = glm::vec3(0.358, 0.254, 0.037);
    static constexpr glm::vec3 _color_high = glm::vec3(0.235, 0.863, 0.216);

    void create_base_mesh(uint nvertices, const Rect& bounds);
};

