#version 330 core

layout (location = 0) in vec3 iPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    gl_Position = uProj * uView * uModel * vec4(iPos, 1.0);
}
