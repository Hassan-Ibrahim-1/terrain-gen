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
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    Terrain terrain;
    terrain.generate();

    Shader& shader = renderer.shaders.base_model;

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

        ImGui::Begin("info");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / InputHandler::io->Framerate, InputHandler::io->Framerate);
        ImGui::End();

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

        terrain.render();

        renderer.set_view_matrix(view);
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

