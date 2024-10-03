#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.hpp"

class Window {
public:
    Window(uint width, uint height, const std::string& title);
    ~Window();
    
    uint get_width() const;
    uint get_height() const;
    GLFWwindow* data();
    
private:
    GLFWwindow* _window = nullptr;

    uint _width = 0;
    uint _height = 0;
    std::string _title;

    GLFWwindow* create_window();
    void load_opengl_functions();
};

