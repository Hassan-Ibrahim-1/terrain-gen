#include <glad/glad.h>

#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "error_handler.hpp"

#include "texture2d.hpp"

Texture2D::Texture2D(const std::string& path,
                    TextureType type,
                    bool default_texture_sampling)
    : _path(path), type(type) {
    stbi_set_flip_vertically_on_load(true);
    load(default_texture_sampling);
}

Texture2D::~Texture2D() {
    unload();
}

const std::string& Texture2D::path() {
    return _path;
}

void Texture2D::load(bool default_texture_sampling) {
    if (_texture_loaded) return;

    glGenTextures(1, &ID);

    _data = stbi_load(_path.c_str(), &width, &height, &nr_channels, 0);
    
    if (_data == nullptr) {
        std::stringstream ss;
        ss << "Bad image load - path: " << _path;
        ErrorHandler::error(ss.str());
        return;
    }

    GLenum format;
    if (nr_channels == 1) {
        format = GL_RED;
    }
    else if (nr_channels == 3) {
        format = GL_RGB;
    }
    else if (nr_channels == 4) {
        format = GL_RGBA;
    }
    else {
        printf("nrCHannels: %d\n", nr_channels);
        ErrorHandler::error("Incompatible channel flag");
        return;
    }

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (default_texture_sampling) {
        enable_default_texture_sampling();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(_data);
    _texture_loaded = true;
}

void Texture2D::unload() {
    return;
    if (_texture_loaded) {
        glDeleteTextures(1, &ID);
        _texture_loaded = false;
    }
}

void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::enable_default_texture_sampling() {
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);*/
    /**/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

