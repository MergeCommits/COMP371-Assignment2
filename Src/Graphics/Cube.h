#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "CarPart.h"

class Mesh;

class Cube : public CarPart {
private:
    const float vertices[216] = {
        // Positions         // Normals
        // back face
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 0.0f, 0.0f, // bottom-left
         0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 1.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 1.0f, 1.0f, // top-right
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 0.0f, 0.0f, // bottom-left
        -0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, // 0.0f, 1.0f, // top-left
        // front face
        -0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 0.0f, 0.0f, // bottom-left
         0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 1.0f, 1.0f, // top-right
         0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 1.0f, 1.0f, // top-right
        -0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 0.0f, 1.0f, // top-left
        -0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, // 0.0f, 0.0f, // bottom-left
        // left face
        -0.5f, 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, // 1.0f, 0.0f, // top-right
        -0.5f, 1.0f, -0.5f, -1.0f,  0.0f,  0.0f, // 1.0f, 1.0f, // top-left
        -0.5f, 0.0f, -0.5f, -1.0f,  0.0f,  0.0f, // 0.0f, 1.0f, // bottom-left
        -0.5f, 0.0f, -0.5f, -1.0f,  0.0f,  0.0f, // 0.0f, 1.0f, // bottom-left
        -0.5f, 0.0f,  0.5f, -1.0f,  0.0f,  0.0f, // 0.0f, 0.0f, // bottom-right
        -0.5f, 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, // 1.0f, 0.0f, // top-right
        // right face
         0.5f, 1.0f,  0.5f,  1.0f,  0.0f,  0.0f, // 1.0f, 0.0f, // top-left
         0.5f, 0.0f, -0.5f,  1.0f,  0.0f,  0.0f, // 0.0f, 1.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  1.0f,  0.0f,  0.0f, // 1.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  1.0f,  0.0f,  0.0f, // 0.0f, 1.0f, // bottom-right
         0.5f, 1.0f,  0.5f,  1.0f,  0.0f,  0.0f, // 1.0f, 0.0f, // top-left
         0.5f, 0.0f,  0.5f,  1.0f,  0.0f,  0.0f, // 0.0f, 0.0f, // bottom-left
        // bottom face
        -0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, // 0.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, // 1.0f, 1.0f, // top-left
         0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, // 1.0f, 0.0f, // bottom-left
         0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, // 1.0f, 0.0f, // bottom-left
        -0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, // 0.0f, 0.0f, // bottom-right
        -0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, // 0.0f, 1.0f, // top-right
        // top face
        -0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, // 0.0f, 1.0f, // top-left
         0.5f, 1.0f , 0.5f,  0.0f,  1.0f,  0.0f, // 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, // 1.0f, 1.0f, // top-right
         0.5f, 1.0f,  0.5f,  0.0f,  1.0f,  0.0f, // 1.0f, 0.0f, // bottom-right
        -0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, // 0.0f, 1.0f, // top-left
        -0.5f, 1.0f,  0.5f,  0.0f,  1.0f,  0.0f, // 0.0f, 0.0f  // bottom-left
    };

    Vector3f position;
    // Scales relative to an arbitrary origin.
    Vector3f scaleOrigin;
    Vector3f scale;
    // Rotations relative to an arbitrary origin.
    Vector3f rotationOrigin;
    Vector3f rotation;

    Mesh* mesh;
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;

public:
    Vector4f color;

    Cube(Shader* shd);

    void setPosition(float x, float y, float z) override;
    void addPositionXZ(const Vector2f& vect) override;
    void setScale(float x, float y, float z) override;
    void addScaleOrigin(float sca) override;
    void addRotationX(float bruh) override;
    void addRotationY(float bruh) override;
    void addRotationOriginY(float bruh) override;
    void addRotationZ(float bruh) override;

    void setShader(Shader* shd) override;

    void render() override;
    void render(const Vector3f& origin) override;
};

#endif // CUBE_H_INCLUDED
