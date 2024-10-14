#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "error_handler.hpp"
#include "input_handler.hpp"
#include "renderer.hpp"
#include "terrain.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "globals.hpp"
#include "settings.hpp"
#include "camera.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static float vertices[] = {
     // positions         // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

static void cleanup();

int main() {
    Utils::init();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    #endif

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Bebe");

    glfwSetInputMode(window.data(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (ErrorHandler::had_error) {
        cleanup();
        return -1;
    }

    glfwSetKeyCallback(window.data(), InputHandler::key_callback);
    glfwSetCursorPosCallback(window.data(), InputHandler::mouse_movement_callback);
    glfwSetScrollCallback(window.data(), InputHandler::mouse_scroll_callback);
    glfwSetMouseButtonCallback(window.data(), InputHandler::mouse_button_callback);

    // Enable vsync
    glfwSwapInterval(1);

    // imgui stuff
    const char* glsl_version = "#version 410";

    // LIGHT CUBE VAO
    uint light_vao = 0;
    uint cube_vbo = 0;
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);

    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // imgui context
    ImGuiIO& io = Utils::create_imgui_context();
    InputHandler::io = &io;

    ImGui::StyleColorsDark();

    // setup renderer
    ImGui_ImplGlfw_InitForOpenGL(window.data(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float& delta_time = Globals::delta_time;
    float last_frame = 0.0f;

    Renderer renderer;
    Globals::renderer = &renderer;

    Camera& camera = Globals::camera;
    camera = Camera(glm::vec3(20, 16, 20));
    camera.set_rotation(0.0f, -35.0f);

    Terrain terrain;
    terrain.generate(1000000, Rect(Transform(glm::vec3(0), glm::vec3(60, 0, 60.0f)), glm::vec4(1)));

    Shader& shader = renderer.shaders.light_model;
    Shader& light_cube_shader = renderer.shaders.light_cube;

    glm::vec3 material_color(1);
    glm::vec3 light_pos(glm::vec3(50, 10, 11));
    glm::vec3 ambient = glm::vec3(1);
    glm::vec3 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec3 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool attenuation_enabled = true;
    float atten_const = 1;
    float atten_linear = -0.08f;
    float atten_quadratic = 0.002f;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.data())) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // NOTE: Have to call this for mouse updates
        InputHandler::process_input(window.data());

        if (glfwGetWindowAttrib(window.data(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        if (Settings::wireframe_enabled) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (Settings::cursor_enabled) {
            glfwSetInputMode(window.data(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.data(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (Settings::cursor_enabled) {
            ImGui::Begin("config");
            ImGui::DragFloat3("light pos", (float*)&light_pos);
            ImGui::ColorEdit3("light ambient", (float*)&ambient);
            ImGui::ColorEdit3("light diffuse", (float*)&diffuse);
            ImGui::ColorEdit3("light specular", (float*)&specular);
            ImGui::ColorEdit3("material color", (float*)&material_color);
            ImGui::Checkbox("attenuation", &attenuation_enabled);
            if (attenuation_enabled) {
                ImGui::DragFloat("attenuation constant", &atten_const, 0.01);
                ImGui::DragFloat("attenuation linear", &atten_linear, 0.01);
                ImGui::DragFloat("attenuation quadratic", &atten_quadratic, 0.01);
            }
            // fps
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.1f, 100.0f);
        glm::mat4 view = camera.get_view_matrix();
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", view);

        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0));
        model = glm::scale(model, glm::vec3(1));
        shader.set_mat4("model", model);

        // LIGHTING
        
        std::string str = "pointlight";
        shader.set_vec3(str + ".position", light_pos);
        shader.set_vec3(str + ".ambient", ambient);
        shader.set_vec3(str + ".diffuse", diffuse);
        shader.set_vec3(str + ".specular", specular);
        shader.set_float("material.shininess", 256.0f);
        shader.set_float(str + ".constant", atten_const);
        shader.set_float(str + ".linear", atten_linear);
        shader.set_float(str + ".quadratic", atten_quadratic);
        shader.set_bool("attenuation_enabled", attenuation_enabled);

        terrain.render();


        light_cube_shader.use();
        light_cube_shader.set_vec3("light_color", 1, 1, 1);
        light_cube_shader.set_mat4("projection", projection);
        glm::mat4 light_model(1.0f);
        light_model = glm::translate(light_model, light_pos);
        light_model = glm::scale(light_model, glm::vec3(1.f));
        light_cube_shader.set_mat4("model", light_model);

        renderer.set_view_matrix(view);

        glBindVertexArray(light_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        renderer.render();

        // render imgui windows
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window.data());
        glfwPollEvents();
    }
    cleanup();

    return 0;
}

static void cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

