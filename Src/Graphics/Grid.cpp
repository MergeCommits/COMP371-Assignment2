#include "Grid.h"

Grid::Grid(Shader* shd) : Mesh(shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Grid(Shader* shd).");
    }
    primitiveCount = 2;
    primitives = {
        0, 2, 3,
        3, 1, 0
    };

    worldMat = shd->getMat4Uniform("modelMatrix");
    color = Vector4f(1.f, 1.f, 1.f, 1.f);
    scale = Vector3f::one;
}

void Grid::generateData() {
    vertexData.clear();

    for (int i = 0; i < 12; i++) {
        vertexData.push_back(vertices[i]);
    }
}

void Grid::uploadData() {
    GLuint err = GL_NO_ERROR;

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to buffer vertex data!");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitives.size()*sizeof(GLuint), primitives.data(), GL_STATIC_DRAW);
}

void Grid::renderInternal() {
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, scale, Vector3f::zero);
    worldMat->setValue(mat);
    shader->getVector4fUniform("fsColor")->setValue(color);
}
