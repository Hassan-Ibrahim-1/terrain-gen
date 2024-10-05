#pragma once

#include "mesh.hpp"

class Terrain {
public:
    Terrain();

    void generate();
    void render();

private:
    Mesh _mesh;
};

