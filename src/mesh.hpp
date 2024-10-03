#pragma once

#include <vector>
#include "shader.hpp"
#include "texture2d.hpp"
#include "vertex.hpp"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    // assumes the texture has been loaded
    std::vector<Texture2D> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices,
         std::vector<Texture2D> textures);
    
    void draw(Shader& shader);

private:
    uint _vao;
    uint _vbo;
    uint _ebo;

    void init_mesh();
};

