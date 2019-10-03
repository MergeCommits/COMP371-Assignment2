#ifndef AXIS_H_INCLUDED
#define AXIS_H_INCLUDED

#include "Shader.h"

class Mesh;

class Axis {
private:
    const float vertices[6] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, -1.f
    };

    Mesh* mesh;
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;

public:
    Vector3f rotation;
    Vector4f color;

    Axis(Shader* shd);
    
    void render();
};

#endif // AXIS_H_INCLUDED
