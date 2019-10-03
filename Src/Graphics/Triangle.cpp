#include <GL/glew.h>
#include <stdexcept>

#include "Triangle.h"

Triangle::Triangle(Shader* shd) : Mesh(shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Triangle(Shader* shd).");
    }
    primitiveCount = 1;
    primitives.push_back(0);
    primitives.push_back(1);
    primitives.push_back(2);
    
    worldMat = shd->getMat4Uniform("modelMatrix");
}

void Triangle::generateData() {
    vertexData.clear();
    
    vertexData.push_back(vertices[0]);
    vertexData.push_back(vertices[1]);
    vertexData.push_back(vertices[2]);
    vertexData.push_back(vertices[3]);
    vertexData.push_back(vertices[4]);
    vertexData.push_back(vertices[5]);
    vertexData.push_back(vertices[6]);
    vertexData.push_back(vertices[7]);
    vertexData.push_back(vertices[8]);
}

void Triangle::uploadData() {
    GLuint err = GL_NO_ERROR;
    
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to buffer vertex data!");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitives.size()*sizeof(GLuint), primitives.data(), GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to buffer index data!");
    }
}

void Triangle::renderInternal() {
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, Vector3f::one, Vector3f::zero);
    worldMat->setValue(mat);
    shader->getVector4fUniform("fsColor")->setValue(Vector4f(1.f, 0.f, 0.f, 1.f));
}
