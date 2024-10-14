#pragma once

#include <glm/glm.hpp>

#include "transform.hpp"

enum class CameraDirection {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
};

namespace CameraDefaults {
    constexpr float fov = 45.0f;
    const glm::vec2 sensitivity(0.1f, 0.1f);
    constexpr float velocity = 23.0f;
    const Rotation rotation(-90.0f, 0.0f, 0.0f);
    const glm::vec3 front(0.0f, 0.0f, -1.0f);
}

class Camera {
public:
    Transform transform;

    glm::vec3 world_up;

    glm::vec3 up;
    glm::vec3 front = CameraDefaults::front;
    glm::vec3 right;

    float fov = CameraDefaults::fov;
    glm::vec2 sensitivity = CameraDefaults::sensitivity;
    float velocity = CameraDefaults::velocity;

    // Transform init
    Camera(Transform transform,
           glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f));

    // vector init
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           Rotation rotation = CameraDefaults::rotation,
           glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec3 world_up = glm::vec3(0.0f, 0.1f, 0.0f));

    // float init
    Camera(float pos_x,
           float pos_y,
           float pos_z,
           float yaw,
           float pitch,
           float roll,
           float scale_x,
           float scale_y,
           float scale_z,
           float world_up_x,
           float world_up_y,
           float world_up_z);

    glm::mat4 get_view_matrix();
    void process_keyboard(CameraDirection direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true, bool invert_pitch = false);
    void process_mouse_scroll(float y_offset);

    // Points the camera backwords
    void look_back();
    void set_rotation(float yaw, float pitch);

private:
    void update_vectors();
};

