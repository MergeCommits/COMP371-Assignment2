#include <cmath>

#include "Camera.h"
#include "../Math/MathUtil.h"

Camera::Camera(float aspectRatio, float fov) {
    position = Vector3f(0.f, 0.f, 0.f);
    lookAt = Vector3f(0.f, 0.f, 1.f);
    upDir = Vector3f(0.f, 1.f, 0.f);

    viewMatrix = Matrix4x4f::constructViewMat(position, lookAt, upDir);

    xAngle = 0.f;
    yAngle = 0.f;
    yAngleLimit = MathUtil::PI / 2.f;
    tilt = 0.f;

    float nearZ = 0.01f;
    float farZ = 25.f;
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    projectionMatrix = Matrix4x4f::constructPerspectiveMat(fov, aspectRatio, nearZ, farZ);

    rotation = Matrix4x4f::identity;

    needsViewUpdate = true;
    needsProjUpdate = true;
}

Camera::Camera(float aspectRatio) : Camera(aspectRatio, MathUtil::degToRad(70.f)) { }

void Camera::addShader(Shader* shd) {
    shaders.push_back(shd);

    needsViewUpdate = true;
    needsProjUpdate = true;
}

void Camera::update() {
    GLuint err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Camera::update().");
    }
    if (needsViewUpdate) {
        rotation = Matrix4x4f::constructWorldMat(Vector3f(0.f, 0.f, 0.f), Vector3f(1.f, 1.f, 1.f), Vector3f(-yAngle, xAngle, tilt));

        viewMatrix = Matrix4x4f::constructViewMat(position, rotation.transform(lookAt), rotation.transform(upDir));

        // Update shaders.
        for (int i = 0; i < (int)shaders.size(); i++) {
            shaders[i]->getMat4Uniform("viewMatrix")->setValue(viewMatrix);
        }

        needsViewUpdate = false;
    }

    if (needsProjUpdate) {
        float nearZ = 0.01f;
        float farZ = 25.f;
        projectionMatrix = Matrix4x4f::constructPerspectiveMat(fov, aspectRatio, nearZ, farZ);
        
        for (int i = 0; i < (int)shaders.size(); i++) {
            shaders[i]->getMat4Uniform("projectionMatrix")->setValue(projectionMatrix);
        }

        needsProjUpdate = false;
    }
}

void Camera::setPosition(const Vector3f& pos) {
    needsViewUpdate = true;
    position = pos;
}

void Camera::setTilt(float rad) {
    needsViewUpdate = !MathUtil::eqFloats(rad, tilt);
    tilt = rad;
}

void Camera::addAngle(float x, float y) {
    if (MathUtil::eqFloats(x, 0.f) && MathUtil::eqFloats(y, 0.f)) {
        return;
    }

    xAngle += x;
    yAngle -= y;

    if (yAngle < -yAngleLimit) {
        yAngle = -yAngleLimit;
    } else if (yAngle > yAngleLimit) {
        yAngle = yAngleLimit;
    }

    float PI_MUL_2 = MathUtil::PI * 2.f;

    if (xAngle > PI_MUL_2) {
        xAngle -= PI_MUL_2;
    } else if (xAngle < -PI_MUL_2) {
        xAngle += PI_MUL_2;
    }

    needsViewUpdate = true;
}

void Camera::resetAngle() {
    xAngle = 0.f;
    yAngle = 0.f;
    needsViewUpdate = true;
}

void Camera::addFov(float deg) {
    fov += MathUtil::degToRad(deg);
    fov = MathUtil::clampFloat(fov, 0.1f, 2.9f);
    needsProjUpdate = true;
}
