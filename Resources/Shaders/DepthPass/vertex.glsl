#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 position;
in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 depthViewMatrix;
uniform mat4 depthProjectionMatrix;

void main() {
    vec3 dummyNormal = normal;
    gl_Position =  depthProjectionMatrix * depthViewMatrix * vec4(position, 1.0);
}
