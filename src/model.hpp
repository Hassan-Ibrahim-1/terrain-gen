#pragma once

#include <assimp/scene.h>
#include "mesh.hpp"
#include "shader.hpp"

class Model {
public:
    Model(const char* path);
    void draw(Shader& shader);

private:
    std::vector<Mesh> _meshes;
    std::vector<Texture2D> _loaded_textures;
    std::string _dir;

    void load(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> load_material_textures(
        aiMaterial* mat,
        aiTextureType type,
        TextureType texture_type
    );
};

