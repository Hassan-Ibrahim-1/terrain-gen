#include <glm/gtc/matrix_transform.hpp>

#include "circle.hpp"
#include "globals.hpp"

Circle::Circle(Transform transform, glm::vec4 color)
    : transform(transform), color(color) {}

Circle::Circle(glm::vec3 position, glm::vec4 color)
    : Circle(Transform(position), color) {}

void Circle::render() {
    Globals::renderer->draw_circle(*this);
}

float Circle::radius() {
    // HACK: 0.5 is the base number used in Renderer::generate_circle_vertices()
    static constexpr float base_radius = 0.5f;

    glm::vec4 base_point_from_center(base_radius * cos(0), base_radius * sin(0), 0, 0);
    glm::mat4 scale_mat(1);
    scale_mat = glm::scale(scale_mat, transform.scale);
    glm::vec4 scaled_point = Globals::camera.get_view_matrix() * scale_mat * base_point_from_center;
    // assume the circle is at 0, 0
    float radius = glm::distance(scaled_point,  glm::vec4(0));
    return abs(radius);
}

bool operator==(const Circle& c1, const Circle& c2) {
    return (c1.transform == c2.transform)
        && (c1.color == c2.color);
}
bool operator!=(const Circle& c1, const Circle& c2) {
    return (c1.transform != c2.transform)
        || (c1.color != c2.color);
}

