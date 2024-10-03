#include <array>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "renderer.hpp"
#include "error_handler.hpp"
#include "globals.hpp"
#include "transform.hpp"

Renderer::Renderer() {
    generate_circle_vertices();

    init_vbos();
    init_vaos();
    init_shaders();
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &_points_vbo);
    glDeleteBuffers(1, &_rects_vbo);
    glDeleteVertexArrays(1, &_points_vao);
    glDeleteVertexArrays(1, &_rects_vao);
}

void Renderer::draw_point(Point& point) {
    _points.push_back(point);
}

void Renderer::draw_point(glm::vec3 position, glm::vec4 color) {
    _points.push_back(Point(position, color));
}

void Renderer::draw_rect(Rect& rect, DrawMode draw_mode) {
    _rects.push_back(rect);
    _rect_draw_modes.push_back(draw_mode);
}

void Renderer::draw_circle(Circle& circle) {
    _circles.push_back(circle);
}

void Renderer::draw_circle(glm::vec3 position, glm::vec4 color) {
    _circles.push_back(Circle(position, color));
}

void Renderer::draw_line(Line& line) {
    point_to_screen_space(line.p1);
    point_to_screen_space(line.p2);
    _lines.push_back(line);
    push_line_data(line);
}

void Renderer::draw_line(Point p1, Point p2) {
    point_to_screen_space(p1);
    point_to_screen_space(p2);
    Line line(p1, p2);
    line.set_color(p1.color);
    _lines.push_back(line);
    push_line_data(line);
}

void Renderer::reload_shaders() {
    printf("Reloading shaders\n");
    shaders.point.reload();
    shaders.line.reload();
}

void Renderer::set_view_matrix(glm::mat4& view) {
    shaders.point.use();
    shaders.point.set_mat4("view", view);
    shaders.line.use();
    shaders.line.set_mat4("view", view);
}

void Renderer::render() {
    update_vbos();

    shaders.point.use();

    glBindVertexArray(_points_vao);
    for (Point& point : _points) {
        glm::mat4 model = point.mat4();
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", point.color);
        glDrawArrays(GL_POINTS, 0, _points.size());
    }

    glBindVertexArray(_rects_vao);
    for (size_t i = 0; i < _rects.size(); i++) {
        Rect& rect = _rects[i];
        DrawMode draw_mode = _rect_draw_modes[i];

        glm::mat4 model = rect.transform.get_mat4();
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", rect.color);

        if (draw_mode == DrawMode::FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        else if (draw_mode == DrawMode::LINE) {
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
    }

    glBindVertexArray(_circles_vao);
    for (Circle& circle : _circles) {
        glm::mat4 model = circle.transform.get_mat4();
        shaders.point.set_mat4("model", model);
        shaders.point.set_vec4("color", circle.color);
        glDrawArrays(GL_TRIANGLE_FAN, 0, _n_circle_segments + 2);
    }

    glBindVertexArray(_lines_vao);
    for (Line& line : _lines) {
        shaders.line.use();
        glDrawArrays(GL_LINES, 0, _lines.size() * 2);
    }

    _points.clear();
    _rects.clear();
    _rect_draw_modes.clear();
    _circles.clear();
    _lines.clear();
    _line_data.clear();
}

void Renderer::init_vbos() {
    glGenBuffers(1, &_points_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_circles_vbo);
    glGenBuffers(1, &_lines_vbo);
}

void Renderer::init_vaos() {
    // points
    glGenVertexArrays(1, &_points_vao);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &_point_vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // rects
    glGenVertexArrays(1, &_rects_vao);
    glBindVertexArray(_rects_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _rects_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _rect_vertices.size(), _rect_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_rects_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rects_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _rect_indices.size(), _rect_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // circles
    glGenVertexArrays(1, &_circles_vao);
    glBindVertexArray(_circles_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _circles_vbo);
    /*printf("circle vertices size: %zu\n", _circle_vertices.size());*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _circle_vertices.size(), _circle_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &_lines_vao);
    glBindVertexArray(_lines_vao);

    // data is sent every frame
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);
    /*auto* verts = new std::array<float, 2> {*/
    /*    0.0f, 0.1f*/
    /*};*/

    /*glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * _lines.size(), _lines.data(), GL_DYNAMIC_DRAW);*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  _line_data.size(), _line_data.data(), GL_DYNAMIC_DRAW);
    /*glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts->size(), verts->data(), GL_STATIC_DRAW);*/

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
}

void Renderer::update_vbos() {
    /*glBindVertexArray(_lines_vao);*/
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);
    /*_lines.data()*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) *  _line_data.size(), _line_data.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    /*glBufferSubData(GL_ARRAY_BUFFER, 0, _lines.size() * 2 * sizeof(float), _lines.data());*/
}

void Renderer::init_shaders() {
    shaders.point.load("shaders/point_shader.vert", "shaders/point_shader.frag");
    shaders.line.load("shaders/line_shader.vert", "shaders/line_shader.frag");

    if (ErrorHandler::had_error) {
        std::exit(-1);
    }
}

void Renderer::generate_circle_vertices() {
    static constexpr float two_pi = M_PI * 2;
    static constexpr float radius = 0.5f;

    for (size_t i = 0; i < _n_circle_segments; i++) {
        // x, y, z
        _circle_vertices.push_back(_circle_start.x + radius * cos((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(_circle_start.y + radius * sin((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(0);
    }
}

Point& Renderer::point_to_screen_space(Point& point) {
    point.position =
        Globals::camera.get_view_matrix() * point.mat4() * glm::vec4(0, 0, 0, 1.0f);
    return point;
}

void Renderer::push_line_data(Line& line) {
    push_point_data(line.p1);
    push_point_data(line.p2);
}

void Renderer::push_point_data(Point& p) {
    _line_data.push_back(p.position.x);
    _line_data.push_back(p.position.y);
    _line_data.push_back(p.color.r);
    _line_data.push_back(p.color.g);
    _line_data.push_back(p.color.b);
    _line_data.push_back(p.color.a);
}

