#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 depthViewMatrix;
uniform mat4 depthProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
    // The OpenGL shader compiler seems to optimize the input vertex normal out
    // so we just assign it to a dummy so it'll compile with unused normals.
    vec3 dummyNormal = normal;
    gl_Position =  depthProjectionMatrix * depthViewMatrix * modelMatrix * vec4(position, 1.0);
}
