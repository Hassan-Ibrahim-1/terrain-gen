#include "globals.hpp"
#include "mesh.hpp"
#include "terrain.hpp"

Terrain::Terrain() : _mesh() {}

void Terrain::generate() {
    std::vector<Vertex> vertices = {
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f)
    };
    _mesh = Mesh(vertices, {}, {});
    /*_mesh.vertices = vertices;*/
    /*_mesh.load();*/
    /* 0.5f, -0.5f, 0.0f, // bottom right*/
    /*-0.5f, -0.5f, 0.0f, // bottom left*/
}

void Terrain::render() {
    _mesh.render(Globals::renderer->shaders.base_model, MeshRenderMode::ARRAYS);
}

