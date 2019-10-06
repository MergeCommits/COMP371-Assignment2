#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    vec3 test = normal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}
