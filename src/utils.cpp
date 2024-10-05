#include <ctime>

#include "utils.hpp"
#include "input_handler.hpp"

void Utils::init() {
    srand(static_cast<unsigned> (time(0)));
}

glm::vec3 Utils::imvec4_to_glm_vec3(ImVec4& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec4 Utils::imvec4_to_glm_vec4(ImVec4& vec) {
    return glm::vec4(vec.x, vec.y, vec.z, vec.w);
}

ImVec4 Utils::glm_vec3_to_imvec4(glm::vec3& vec) {
    return ImVec4(vec.x, vec.y, vec.z, 0);
}

ImGuiIO& Utils::create_imgui_context() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io; // what
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    return io;
}

void Utils::print_vec3(const glm::vec3& v, const std::string& msg) {
    printf("%s: (%f, %f, %f)\n", msg.c_str(), v.x, v.y, v.z);
}

float Utils::random_float(float min, float max) {
    return min + static_cast<float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

float Utils::noise(int x, int y) {
    int n = x + y * 57;
    n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589)
    & 0x7fffffff) / 1073741824.0); 
}

bool Utils::mouse_in_circle(Circle& circle) {
    return mouse_in_circle(circle.transform.position, circle.radius());
}

bool Utils::mouse_in_circle(glm::vec3 position, float radius) {
    glm::vec2 mouse_p = InputHandler::get_mouse_pos();
    float dist = (mouse_p.x - position.x) * (mouse_p.x - position.x) + (mouse_p.y - position.y) * (mouse_p.y - position.y);
    return dist < radius * radius;
}
