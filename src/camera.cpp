#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "transform.hpp"

Camera::Camera(Transform transform, glm::vec3 world_up)
    : transform(transform),
      world_up(world_up),
      up(world_up) {
    update_vectors();
}

Camera::Camera(glm::vec3 position, Rotation rotation, glm::vec3 scale, glm::vec3 world_up)
    : Camera(Transform(position, scale, rotation), world_up) {}

Camera::Camera(float pos_x,
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
               float world_up_z)
    : Camera(glm::vec3(pos_x, pos_y, pos_z),
             Rotation(yaw, pitch, roll),
             glm::vec3(scale_x, scale_y, scale_z),
             glm::vec3(world_up_x, world_up_y, world_up_z)) {}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(transform.position, transform.position + front, up);
}

void Camera::process_keyboard(CameraDirection direction, float delta_time) {
    static const float vertical_multiplier = 5.0f;

    float speed = velocity * delta_time;

    switch (direction) {
    case CameraDirection::FORWARD:
        transform.position += front * speed;
        break;
    case CameraDirection::BACKWARD:
        transform.position -= front * speed;
        break;
    case CameraDirection::RIGHT:
        transform.position += right * speed;
        break;
    case CameraDirection::LEFT:
        transform.position -= right * speed;
        break;
    case CameraDirection::UP:
        transform.position += world_up * (speed * vertical_multiplier);
        break;
    case CameraDirection::DOWN:
        transform.position -= world_up * (speed * vertical_multiplier);
        break;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch, bool invert_pitch) {
    transform.rotation.yaw += (x_offset * sensitivity.x);
    transform.rotation.pitch += ((invert_pitch ? y_offset : -y_offset) * sensitivity.y);
    /*transform.rotation.yaw += x_offset * sensitivity.x;*/

    if (constrain_pitch) {
        if (transform.rotation.pitch > 89.0f) {
            transform.rotation.pitch = 89.0f;
        }
        else if (transform.rotation.pitch < -89.0f) {
            transform.rotation.pitch = -89.0f;
        }
    }
    update_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
    fov -= y_offset;
    if (fov > 100.0f) {
        fov = 100.0f;
    }
    else if (fov < 45.0f) {
        fov = 45.0f;
    }
}

void Camera::look_back() {
    transform.rotation.yaw = -transform.rotation.yaw;
    update_vectors();
}

void Camera::set_rotation(float yaw, float pitch) {
    transform.rotation.yaw = yaw;
    transform.rotation.pitch = pitch;
    update_vectors();
}

void Camera::update_vectors() {
    // Calculate new front vector - where the camera is facing
    front.x = cos(glm::radians(transform.rotation.yaw)) * cos(glm::radians(transform.rotation.pitch));
    front.y = sin(glm::radians(transform.rotation.pitch));
    front.z = sin(glm::radians(transform.rotation.yaw)) * cos(glm::radians(transform.rotation.pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

