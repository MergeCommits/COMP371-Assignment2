#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/glew.h>

class Shader;

class Mesh {
private:
    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint primitiveBuffer = 0;

protected:
    Shader* shader;
    GLsizei primitiveCount = 0;
    GLenum mode;

    bool needsGen;
    bool needsUpload;
    virtual void generateData()=0;
    virtual void uploadData()=0;
    virtual void renderInternal()=0;

public:
    Mesh(Shader* shd);
    ~Mesh();

    void render();
};

#endif // MESH_H_INCLUDED
