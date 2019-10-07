#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

out vec3 fsPosition;
out vec3 fsNormal;
out vec4 fsPositionLightSpace;

void main() {
    fsPosition = vec3(modelMatrix * vec4(position, 1.0));
    fsNormal = transpose(inverse(mat3(modelMatrix))) * normal;
    // Get the position from the light's perspective.
    fsPositionLightSpace = lightProjectionMatrix * lightViewMatrix * vec4(fsPosition, 1.0);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
