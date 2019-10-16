#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../Math/Matrix.h"
#include "Shader.h"

class GraphicsResources;

class Camera {
private:
    GraphicsResources* gfxRes;

    float xAngle;
    float yAngle;
    // The range of the the yAngle field before it is clamped.
    float yAngleLimit;
    float tilt;

    float nearPlaneZ;
    float farPlaneZ;
    int width;
    int height;
    float fov;
    // Perspective or Orthographic matrix.
    bool orthographicProj;

    bool needsViewUpdate;
    bool needsProjUpdate;

    Vector3f position;
    Vector3f lookAt;
    Vector3f upDir;

    Matrix4x4f viewMatrix;
    Matrix4x4f projectionMatrix;
    Matrix4x4f rotation;

    std::vector<Shader*> shaders;
    
    float getAspectRatio() const;

public:
    Camera(int w, int h, float fov, float nearZ = 0.01f, float farZ = 30.f, bool orthographic = false);
    Camera(int w, int h);

    void addShader(Shader* shd);

    Matrix4x4f getViewMatrix() const;
    Matrix4x4f getProjectionMatrix() const;

    void update();

    Vector3f getPosition() const;
    void setPosition(const Vector3f& pos);
    void setTilt(float rad);
    void addAngle(float xAngle, float yAngle);
    void resetAngle();
    
    void addFov(float deg);
    void setXYClippings(int w, int h);
    void setZClippings(float nearZ, float farZ);
    void setAspectRatio(float ratio);
    void setOrthographicProj(bool bruh);
};

#endif // CAMERA_H_INCLUDED
