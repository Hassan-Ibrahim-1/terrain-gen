#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstddef>
#include <sstream>
#include "model.hpp"
#include "error_handler.hpp"
#include "vertex.hpp"

Model::Model(const char* path) {
    load(path);
}

void Model::draw(Shader& shader) {
    for (auto& mesh : _meshes) {
        mesh.render(shader);
    }
}

void Model::load(std::string path) {
    Assimp::Importer imp;
    const aiScene* scene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::stringstream ss;
        ss << "Model Loading Error: " << imp.GetErrorString() << '\n';
        ErrorHandler::error(ss.str());
    }
    _dir = path.substr(0, path.find_last_of('/'));
    /*std::cout << "dir: " << _dir << '\n';*/
    /*std::exit(0);*/

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        // Node contains indices of corresponding in scene->mMeshes
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(process_mesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}
Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture2D> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        glm::vec2 tex_coords(0);
        if (mesh->mTextureCoords[0]) {
            tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.emplace_back(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            tex_coords
        );
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t  j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture2D> diffuse_maps =
            load_material_textures(
                material,
                aiTextureType_DIFFUSE,
                TextureType::DIFFUSE
            );
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture2D> specular_maps =
            load_material_textures(
                material,
                aiTextureType_SPECULAR,
                TextureType::SPECULAR
            );
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture2D> Model::load_material_textures(
    aiMaterial* mat,
    aiTextureType type,
    TextureType texture_type) {

    // NOTE: Assumes that textures are in the same directory as the model

    std::vector<Texture2D> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString path;
        mat->GetTexture(type, i, &path);
        bool skip = false;
        for (auto& tex : _loaded_textures) {
            if (std::strcmp(tex.path().data(), path.C_Str())) {
                textures.push_back(tex);
                skip = true;
                break;
            }
        }
        if (skip) continue;

        textures.emplace_back(_dir + "/" + path.C_Str(), texture_type);
        _loaded_textures.emplace_back(_dir + "/" + path.C_Str(), texture_type);
    }

    return textures;
}

