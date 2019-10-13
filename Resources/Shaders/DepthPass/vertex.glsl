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
    vec3 dummyNoral = normal;
    vec2 dummyUV = uv;
    gl_Position =  depthProjectionMatrix * depthViewMatrix * modelMatrix * vec4(position, 1.0);
}
