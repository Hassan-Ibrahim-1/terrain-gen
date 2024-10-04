#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "input_handler.hpp"
#include "globals.hpp"
#include "settings.hpp"
#include "camera.hpp"

void InputHandler::process_input(GLFWwindow* window) {
    update_mouse_buttons();

    Camera& camera = Globals::camera;
    float delta_time = Globals::delta_time;

    /*if (Globals::io->WantTextInput) return;*/

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::DOWN, delta_time);
        /*glfwSetWindowShouldClose(window, true);*/
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard(CameraDirection::LEFT, delta_time);
    }
}

glm::vec2 InputHandler::get_mouse_pos() {
    return _mouse_pos;
}

float InputHandler::get_mouse_pos_x() {
    return _mouse_pos.x;
}
float InputHandler::get_mouse_pos_y() {
    return _mouse_pos.y;
}

void InputHandler::update_mouse_buttons() {
    if (_lmb_clicked) {
        if (_lmb_click_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _lmb_clicked = false;
        }
    }
    if (_rmb_clicked) {
        if (_rmb_click_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _rmb_clicked = false;
        }
    }
    if (_lmb_released) {
        if (_lmb_release_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _lmb_released = false;
        }
    }
    if (_rmb_released) {
        if (_rmb_release_start_time + _mouse_discard_wait_time < glfwGetTime()) {
            _rmb_released = false;
        }
    }
}

void InputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (Settings::cursor_enabled) {
            Settings::cursor_enabled = false;
        }
        else {
            glfwSetWindowShouldClose(window, true);
        }
    }

    /*if (Globals::io->WantTextInput) {*/
    /*    return;*/
    /*}*/
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        Settings::wireframe_enabled = !Settings::wireframe_enabled;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        Globals::renderer->reload_shaders();
    }
    // enable / disable cursor
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        Settings::cursor_enabled = !Settings::cursor_enabled;
    }
}

void InputHandler::mouse_movement_callback(GLFWwindow* window, double posx, double posy) {

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    /*Globals::mouse_pos.x = posx / width;*/
    /*Globals::mouse_pos.y = posy / height;*/

    glm::vec3 win(posx, posy, 0);
    glm::vec4 viewport(0, 0, width, height);
    glm::vec3 real_pos = glm::unProject(win, glm::mat4(1.0f), glm::mat4(1.0f), viewport);

    _mouse_pos.x = real_pos.x;
    _mouse_pos.y = -real_pos.y;

    if (Settings::cursor_enabled) {
        _last_mouse_x = posx;
        _last_mouse_y = posy;
        return;
    }
    if (_first_mouse) {
        _last_mouse_x = posx;
        _last_mouse_y = posy;
        _first_mouse = false;
    }
    float x_offset = posx - _last_mouse_x;
    float y_offset = posy - _last_mouse_y;
    _last_mouse_x = posx;
    _last_mouse_y = posy;

    Globals::camera.process_mouse_movement(x_offset, y_offset);
}

void InputHandler::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (Settings::cursor_enabled) return;

    Globals::camera.process_mouse_scroll(yoffset);
}

void InputHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (io->WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            _lmb_down = true;
            _lmb_clicked = true;
            _lmb_released = false;
            _lmb_click_start_time = glfwGetTime();
        }
        else if (action == GLFW_RELEASE) {
            _lmb_down = false;
            _lmb_clicked = false;
            _lmb_released = true;
            _lmb_release_start_time = glfwGetTime();
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            _rmb_down = true;
            _rmb_clicked = true;
            _rmb_released = false;
            _rmb_click_start_time = glfwGetTime();
        }
        else if (action == GLFW_RELEASE) {
            _rmb_down = false;
            _rmb_clicked = false;
            _rmb_released = true;
            _rmb_release_start_time = glfwGetTime();
        }
    }
}

bool InputHandler::mouse_button_down(MouseButton button) {
    if (button == MouseButton::ANY) {
        return _lmb_down || _rmb_down;
    }
    else if (button == MouseButton::LEFT) {
        return _lmb_down;
    }
    else if (button == MouseButton::RIGHT) {
        return _rmb_down;
    }
    /*if (button == MouseButton::ANY) {*/
    /*    if (_lmb_down) {*/
    /*        _lmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    if (_rmb_down) {*/
    /*        _rmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /*else if (button == MouseButton::LEFT) {*/
    /*    if (_lmb_down) {*/
    /*        _lmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /*else if (button == MouseButton::RIGHT) {*/
    /*    if (_rmb_down) {*/
    /*        _rmb_down = false;*/
    /*        return true;*/
    /*    }*/
    /*    return false;*/
    /*}*/
    /**/
    // never reached
    return false;
}

bool InputHandler::mouse_button_clicked(MouseButton button) {
    if (button == MouseButton::ANY) {
        if (_lmb_clicked) {
            _lmb_clicked = false;
            return true;
        }
        if (_rmb_clicked) {
            _rmb_clicked = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::LEFT) {
        if (_lmb_clicked) {
            _lmb_clicked = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::RIGHT) {
        if (_rmb_clicked) {
            _rmb_clicked = false;
            return true;
        }
        return false;
    }
    
    // never reached
    return false;
}

bool InputHandler::mouse_button_released(MouseButton button) {
    if (button == MouseButton::ANY) {
        if (_lmb_released) {
            _lmb_released = false;
            return true;
        }
        if (_rmb_released) {
            _rmb_released = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::LEFT) {
        if (_lmb_released) {
            _lmb_released = false;
            return true;
        }
        return false;
    }
    else if (button == MouseButton::RIGHT) {
        if (_rmb_released) {
            _rmb_released = false;
            return true;
        }
        return false;
    }
    
    // never reached
    return false;
}

