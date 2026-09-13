#version 330 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vColor;

out vec4 FragColor;

const int MAX_DIR_LIGHTS = 8;
const int MAX_POINT_LIGHTS = 8;

uniform int uDirLightCount;
uniform vec3 uDirLightDirections[MAX_DIR_LIGHTS];
uniform vec3 uDirLightColors[MAX_DIR_LIGHTS];

uniform int uPointLightCount;
uniform vec3 uPointLightPositions[MAX_POINT_LIGHTS];
uniform vec3 uPointLightColors[MAX_POINT_LIGHTS];
uniform float uPointLightRanges[MAX_POINT_LIGHTS];

uniform vec3 uViewPos;

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 albedo = vColor;
    vec3 lighting = 0.08 * albedo;

    for (int i = 0; i < uDirLightCount && i < MAX_DIR_LIGHTS; ++i) {
        vec3 L = normalize(-uDirLightDirections[i]);
        float diff = max(dot(N, L), 0.0);
        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), 32.0);
        vec3 radiance = uDirLightColors[i];
        lighting += (diff * albedo + spec) * radiance;
    }

    for (int i = 0; i < uPointLightCount && i < MAX_POINT_LIGHTS; ++i) {
        vec3 toLight = uPointLightPositions[i] - vWorldPos;
        float dist = length(toLight);
        vec3 L = toLight / max(dist, 1e-4);
        float range = max(uPointLightRanges[i], 1e-4);
        float atten = clamp(1.0 - dist / range, 0.0, 1.0);
        atten *= atten;

        float diff = max(dot(N, L), 0.0);
        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), 32.0);
        vec3 radiance = uPointLightColors[i] * atten;
        lighting += (diff * albedo + spec) * radiance;
    }

    FragColor = vec4(lighting, 1.0);
}
