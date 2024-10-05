#pragma once

#include <vector>
#include "shader.hpp"
#include "texture2d.hpp"
#include "vertex.hpp"

enum class MeshRenderMode {
    ARRAYS, // render using glDrawArrays
    INDICES, // render using glDrawElements
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    // assumes the texture has been loaded
    std::vector<Texture2D> textures;

    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices,
         std::vector<Texture2D> textures);
    
    void load();
    void reset();
    void render(Shader& shader, MeshRenderMode render_mode = MeshRenderMode::INDICES);

private:
    uint _vao;
    uint _vbo;
    uint _ebo;

    void init_mesh();
};

