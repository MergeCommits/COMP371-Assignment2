#version 330 core

uniform sampler2D tex0;

in vec2 fsUV;

out vec4 outColor;

void main() {
    outColor = vec4(texture(tex0, fsUV).r);
//    outColor = vec4(1.0);
}
