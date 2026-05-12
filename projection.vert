#version 330 core
layout (location = 0) in vec2 aPos;
uniform mat4 MVP;
uniform vec2 movePositionShift;
uniform float scale;

void main() {
    gl_Position = MVP * vec4(scale * aPos + movePositionShift, 0.0, 1.0);
}