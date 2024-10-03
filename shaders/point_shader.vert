#version 410 core

layout (location = 0) in vec3 a_position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; // converts vectors to world_space

void main() {
    vec4 pos = view * model * vec4(a_position, 1.0f);
    pos.z = 0.0f;
    gl_Position = pos;
}

