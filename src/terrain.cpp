#include <strings.h>
#include <glm/gtc/noise.hpp>
#include "globals.hpp"
#include "mesh.hpp"
#include "terrain.hpp"

Terrain::Terrain() {}

void Terrain::generate(uint nvertices, const Rect& bounds) {
    create_base_mesh(nvertices, bounds);
}

void Terrain::render() {
    _mesh.render(Globals::renderer->shaders.base_model, MeshRenderMode::INDICES);
}

void Terrain::create_base_mesh(uint nvertices, const Rect& bounds) {
    uint f1 = (uint) glm::sqrt(nvertices);
    while (nvertices % f1 != 0) {
        f1--;
    }
    uint f2 = nvertices / f1;

    uint nrows = 0;
    uint ncols = 0;

    if (bounds.transform.scale.x > bounds.transform.scale.z) {
        nrows = f1 - 1;
        ncols = f2 - 1;
    }
    else {
        nrows = f2 - 1;
        ncols = f1 - 1;
    }

    Transform vert_t;
    vert_t.scale.x = bounds.transform.scale.x / ncols;
    vert_t.scale.z = bounds.transform.scale.z / nrows;

    vert_t.position.x = bounds.transform.scale.x + bounds.transform.position.x;
    vert_t.position.x += vert_t.scale.x / 2;

    vert_t.position.z = bounds.transform.scale.x + bounds.transform.position.z;
    vert_t.position.z -= vert_t.scale.z / 2;

    float originalx = vert_t.position.x;

    std::vector<Vertex> vertices;
    std::vector<uint> indices;

    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            /*vert_t.position.y = Utils::noise(vert_t.position.x, vert_t.position.z);*/
            vert_t.position.y = glm::perlin(glm::vec2(vert_t.position.x / 2, vert_t.position.z / 2));
            /*vert_t.position.y = glm::simplex(glm::vec2(vert_t.position.x, vert_t.position.z));*/
            glm::vec3 color(1);
            if (vert_t.position.y < _color_boundary) {
                color = _color_low;
            }
            else {
                color = _color_high;
            }
            vertices.emplace_back(vert_t.position, glm::vec3(0), glm::vec2(0), color);
            vert_t.position.x += vert_t.scale.x;
        }
        vert_t.position.x = originalx;
        vert_t.position.z -= vert_t.scale.z;
    }

    for (size_t j = 0; j < nrows - 1; j++) {
        for (size_t i = 0; i < ncols - 1; i++) {
            size_t top_left = j * ncols + i;
            size_t top_right = top_left + 1;
            size_t bottom_left = top_left + ncols;
            size_t bottom_right = bottom_left + 1;

            indices.push_back(top_left);
            indices.push_back(top_right);
            indices.push_back(bottom_left);

            indices.push_back(bottom_left);
            indices.push_back(top_right);
            indices.push_back(bottom_right);

            // Calculate normals
            /*glm::vec3 normal = glm::normalize(*/
            /*    glm::cross(vertices[top_right].position - vertices[top_left].position,*/
            /*             vertices[bottom_left].position - vertices[top_left].position));*/
            glm::vec3 normal =
                glm::cross(vertices[top_right].position - vertices[top_left].position,
                         vertices[bottom_left].position - vertices[top_left].position);
            vertices[top_left].normal += normal;
            vertices[top_right].normal += normal;
            vertices[bottom_left].normal += normal;
            /*vertices[top_left].normal = normal;*/
            /*vertices[top_right].normal = normal;*/
            /*vertices[bottom_left].normal = normal;*/
            /*vertices[bottom_right].normal += normal;*/
        }
    }

    /*for (auto& vertex : vertices) {*/
    /*    vertex.normal = glm::normalize(vertex.normal);*/
    /*}*/

    _mesh = Mesh(vertices, indices, {});
}

