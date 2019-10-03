#include "Axis.h"

Axis::Axis(Shader* shd) : Mesh(shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Axes(Shader* shd).");
    }
    mode = GL_LINES;
    primitiveCount = 1;
    primitives = {
        0, 1
    };

    worldMat = shd->getMat4Uniform("modelMatrix");
    rotation = Vector3f::zero;
    color = Vector4f::one;
}

void Axis::generateData() {
    vertexData.clear();

    for (int i = 0; i < 6; i++) {
        vertexData.push_back(vertices[i]);
    }
}

void Axis::uploadData() {
    GLuint err = GL_NO_ERROR;

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to buffer vertex data!");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitives.size()*sizeof(GLuint), primitives.data(), GL_STATIC_DRAW);
}

void Axis::renderInternal() {
//   Matrix4x4f mat = Matrix4x4f::rotate(rotation);
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, Vector3f(2.f, 2.f, 2.f), rotation);
    worldMat->setValue(mat);
    shader->getVector4fUniform("fsColor")->setValue(color);
}
