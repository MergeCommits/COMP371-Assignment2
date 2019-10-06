#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>

#include "Shader.h"

class Mesh;

class Grid {
private:
    const float vertices[12 * 2] = {
        // Positions     // Normals
        -1.f, 0.f, -1.f, 0.f, 1.f, 0.f,
        -1.f, 0.f, 1.f,  0.f, 1.f, 0.f,
        1.f, 0.f, -1.f,  0.f, 1.f, 0.f,
        1.f, 0.f, 1.f,   0.f, 1.f, 0.f
    };
    
    Mesh* mesh;
    
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;

public:
    Vector4f color;
    Vector3f scale;

    Grid(Shader* shd);
    
    void setShader(Shader* shd);
    void render();
};

#endif // GRID_H_INCLUDED
