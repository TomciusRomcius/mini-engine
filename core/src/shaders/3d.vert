#version 330 core

layout (location = 0) in vec3 iPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vColor;

void main() {
    vec4 worldPos = uModel * vec4(iPos, 1.0);
    vColor = iPos * 0.5 + 0.5;
    gl_Position = uProj * uView * worldPos;
}
