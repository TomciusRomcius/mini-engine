#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vWorldPos;
out vec3 vNormal;
out vec3 vColor;

void main() {
    vec4 worldPos = uModel * vec4(iPos, 1.0);
    vWorldPos = worldPos.xyz;
    vNormal = mat3(transpose(inverse(uModel))) * iNormal;
    vColor = iPos * 0.5 + 0.5;
    gl_Position = uProj * uView * worldPos;
}
