#version 410 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

out vec4 color;

uniform mat4 view;

void main() {
    vec4 pos = view * vec4(a_position, 0.0f, 1.0f);
    pos.z = 0;
    gl_Position = pos;
    color = a_color;
}

