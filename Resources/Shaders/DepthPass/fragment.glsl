#version 330 core

out float outDepth;

void main(){
    // Not really needed, OpenGL does it anyway.
    outDepth = gl_FragCoord.z;
}
