#pragma once

#include <glm/glm.hpp>
#include <string>
#include "imgui.h"

#include "transform.hpp"
#include "circle.hpp"

namespace Utils {

void init();

glm::vec3 imvec4_to_glm_vec3(ImVec4& vec);
glm::vec4 imvec4_to_glm_vec4(ImVec4& vec);

ImVec4 glm_vec3_to_imvec4(glm::vec3& vec);
ImGuiIO& create_imgui_context();

void print_vec3(const glm::vec3& v, const std::string& msg = "");

float random_float(float min, float max);

float noise(int x, int y);

// TODO:
void add_to_imgui(Transform& transform);

bool mouse_in_circle(glm::vec3 position, float radius);
bool mouse_in_circle(Circle& circle);

}

