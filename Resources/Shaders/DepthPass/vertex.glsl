#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 depthViewMatrix;
uniform mat4 depthProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
    // The OpenGL shader compiler optimizes out unused input variables
    // so we just assign it them a dummy so it'll compile with unused vars.
    vec4 dummyNormal = vec4(clamp(normal.x, 0.0f, 0.0f), 0.0, 0.0, 0.0);
    vec4 dummyUV = vec4(clamp(uv.x, 0.0f, 0.0f), 0.0, 0.0, 0.0);
    gl_Position =  depthProjectionMatrix * depthViewMatrix * modelMatrix * vec4(position, 1.0) + dummyUV + dummyNormal;
}
