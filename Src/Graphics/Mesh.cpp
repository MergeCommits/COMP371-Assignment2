#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(Shader* shd) {
    shader = shd;
    mode = GL_TRIANGLES;

    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &primitiveBuffer);
    glGenVertexArrays(1, &vertexArrayObject);

    needsGen = true;
    needsUpload = true;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &primitiveBuffer);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::render() {
    GLuint err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Mesh::render().");
    }

    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitiveBuffer);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to bind vertex buffers!");
    }

    if (needsGen) { generateData(); needsGen = false; }
    if (needsUpload) { uploadData(); needsUpload = false; }

    renderInternal();
    shader->use();

    int primitiveMultiplier = 3;
    if (mode == GL_LINES) { primitiveMultiplier = 2; }
    else if (mode != GL_TRIANGLES) {
        throw std::runtime_error("Unsupported geometry type!");
    }

    glDrawElements(mode, primitiveCount * primitiveMultiplier, GL_UNSIGNED_INT, nullptr);

    shader->unbindVertexInputs();
    glBindVertexArray(0);
}
