#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include <vector>

#include "Mesh.h"
#include "Shader.h"

class Triangle : public Mesh {
private:
    const float vertices[9] = {
        0.f, 0.5f, 0.f,    // Top.
        -0.5f, -0.5f, 0.f, // Left.
        0.5f, -0.5f, 0.f   // Right.
    };

    std::vector<GLuint> primitives;

    std::vector<float> vertexData;
    
    Shader::Uniform* worldMat;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;
    virtual void renderInternal() override;

public:
    Triangle(Shader* shd);
};

#endif // TRIANGLE_H_INCLUDED
