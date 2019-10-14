#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

#include "Shader.h"
#include "CarPart.h"

class Mesh;

class Wheel : public CarPart {
private:
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
    
    Wheel(Shader* shd);

    void setPosition(float x, float y, float z) override;
    void addPositionXZ(const Vector2f& vect) override;
    void setScale(float x, float y, float z) override;
    void addScaleOrigin(float sca) override;
    void addRotationX(float bruh) override;
    void addRotationY(float bruh) override;
    void addRotationOriginY(float bruh) override;
    void addRotationZ(float bruh) override;

    void setShader(Shader* shd) override;
    
    bool isWheel() override { return true; }

    void render() override;
    void render(const Vector3f& origin) override;
};

#endif // WHEEL_H_INCLUDED
