#version 410 core

out vec4 FragColor;

uniform vec4 color;

void main() {
    // FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    FragColor = color;
}

