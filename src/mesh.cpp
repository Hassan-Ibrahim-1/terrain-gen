#include <glad/glad.h>
#include "mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices,
         const std::vector<Texture2D>& textures)
    : vertices(vertices), indices(indices), textures(textures) {
    init_mesh();
}

void Mesh::load() {
    init_mesh();
}

void Mesh::reset() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
    vertices.clear();
    indices.clear();
    textures.clear();
}

void Mesh::render(Shader& shader, MeshRenderMode render_mode) {
    // Assumes that texture names are formatted as such in the shader:
    // material.texture_diffuse1
    // material.texture_diffuse2
    // material.texture_specular1
    // material.texture_specular2
    uint ndiffuse = 1;
    uint nspecular = 1;
    for (size_t i = 0; i < textures.size(); i++) {
        Texture2D& texture = textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name;
        if (texture.type == TextureType::DIFFUSE) {
            name = "texture_diffuse";
            number = std::to_string(ndiffuse++);
        } 
        else if (texture.type == TextureType::SPECULAR) {
            name = "texture_specular";
            number = std::to_string(nspecular++);
        }

        // shader.use();
        shader.set_int("material." + name + number, i);
        texture.bind();
    }
    glBindVertexArray(_vao);
    if (render_mode == MeshRenderMode::ARRAYS) {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    else if (render_mode == MeshRenderMode::INDICES) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::init_mesh() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Tex coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    // color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

