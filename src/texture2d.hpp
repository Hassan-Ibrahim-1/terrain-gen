#pragma once

#include <glad/glad.h>

#include <string>

#include "common.hpp"

enum class TextureType {
    DIFFUSE,
    SPECULAR,
};

class Texture2D {
public:
    uint ID = 0;
    int width = 0;
    int height = 0;
    int nr_channels = 0;
    TextureType type;

    // automatically loads texture
    Texture2D(const std::string& path,
              TextureType type,
              bool default_texture_sampling = true);
    // For Already loaded textured

    // FIXME: THIS SHOULD NOT UNLOAD TEXTURES
    ~Texture2D(); // automatically unloads texture

    const std::string& path();
    // manually unload a texture
    void unload();
    
    // Binds the texture in opengl
    void bind();

    // Enables some default texture sampling in opengl
    void enable_default_texture_sampling();

private:
    std::string _path;
    u8* _data;
    bool _texture_loaded = false;

    // automatically generates a mipmap
    void load(bool default_texture_sampling);
};

