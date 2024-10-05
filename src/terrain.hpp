#pragma once

#include "mesh.hpp"

class Terrain {
public:
    Terrain();

    void generate(uint nvertices, const Rect& bounds);
    void render();

private:
    Mesh _mesh;

    void create_base_mesh(uint nvertices, const Rect& bounds);
};

