#include "Grid.h"
#include "Mesh.h"

Grid::Grid(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Grid(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    std::vector<float> verts;
    for (int i = 0; i < 12; i++) {
        verts.push_back(vertices[i]);
    }
    std::vector<int> prims = {
        0, 2, 3,
        3, 1, 0
    };
    
    mesh->setGeometry(verts, prims);

    worldMat = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVector4fUniform("fsColor");
    color = Vector4f(1.f, 1.f, 1.f, 1.f);
    scale = Vector3f::one;
}

void Grid::render() {
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, scale, Vector3f::zero);
    worldMat->setValue(mat);
    colorUniform->setValue(color);
    
    mesh->render();
}
