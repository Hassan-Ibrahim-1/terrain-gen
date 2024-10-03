#include "error_handler.hpp"

#include "window.hpp"

static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(uint width, uint height, const std::string& title)
    : _width(width), _height(height), _title(title) {
    _window = create_window();
    glfwMakeContextCurrent(_window);
    // Sets a default basic frame buffer size callback
    glfwSetFramebufferSizeCallback(_window, default_framebuffer_size_callback);
    load_opengl_functions();
}

Window::~Window() {
    glfwDestroyWindow(_window);
}

uint Window::get_width() const {
    return _width;
}

uint Window::get_height() const {
    return _height;
}

GLFWwindow* Window::data() {
    return _window;
}

GLFWwindow* Window::create_window() {
    GLFWwindow* window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    if (window == nullptr) {
        ErrorHandler::error("Bad window initialization.");
    }
    return window;
}

void Window::load_opengl_functions() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ErrorHandler::error("Bad glad initialization.");
    }
}

static void default_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

