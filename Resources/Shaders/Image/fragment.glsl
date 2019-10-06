#version 330 core

uniform sampler2D tex0;

in vec2 fsUV;

out vec4 outColor;

void main() {
//    outColor = texture(tex0, fsUV);
    outColor = vec4(1.0);
}
