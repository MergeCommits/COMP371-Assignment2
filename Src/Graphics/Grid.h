#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>

#include "Mesh.h"
#include "Shader.h"

class Grid : public Mesh {
private:
    const float vertices[12] = {
        -1.f, 0.f, -1.f,
        -1.f, 0.f, 1.f,
        1.f, 0.f, -1.f,
        1.f, 0.f, 1.f
    };

    std::vector<GLuint> primitives;

    std::vector<float> vertexData;

    Shader::Uniform* worldMat;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;
    virtual void renderInternal() override;

public:
    Vector4f color;
    Vector3f scale;

    Grid(Shader* shd);
};

#endif // GRID_H_INCLUDED
