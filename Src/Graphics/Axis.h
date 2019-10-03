#ifndef AXIS_H_INCLUDED
#define AXIS_H_INCLUDED

#include "Mesh.h"
#include "Shader.h"

class Axis : public Mesh {
private:
    const float vertices[6] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, -1.f
    };

    std::vector<GLuint> primitives;

    std::vector<float> vertexData;

    Shader::Uniform* worldMat;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;
    virtual void renderInternal() override;

public:
    Vector3f rotation;
    Vector4f color;

    Axis(Shader* shd);
};

#endif // AXIS_H_INCLUDED
