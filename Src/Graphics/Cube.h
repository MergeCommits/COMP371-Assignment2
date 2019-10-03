#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "Mesh.h"
#include "Shader.h"

class Cube : public Mesh {
private:
    const float vertices[24] = {
        -0.5f, 0.f, -0.5f,
        0.5f, 0.f, -0.5f,
        0.5f, 1.f, -0.5f,
        -0.5f, 1.f, -0.5f,
        -0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.5f, 1.f, 0.5f,
        -0.5f, 1.f, 0.5f
    };

    std::vector<GLuint> primitives;

    std::vector<float> vertexData;

    Vector3f position;
    // Scales relative to an arbitrary origin.
    Vector3f scaleOrigin;
    Vector3f scale;
    // Rotations relative to an arbitrary origin.
    Vector3f rotationOrigin;
    Vector3f rotation;
    Shader::Uniform* worldMat;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;
    virtual void renderInternal() override;

public:
    Vector4f color;

    Cube(Shader* shd);

    void setPosition(float x, float y, float z);
    void addPositionXZ(const Vector2f& vect);
    void setScale(float x, float y, float z);
    void addScaleOrigin(float sca);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    void addRotationOriginY(float bruh);
    void addRotationZ(float bruh);
    
    void render(const Vector3f& origin);
};

#endif // CUBE_H_INCLUDED
