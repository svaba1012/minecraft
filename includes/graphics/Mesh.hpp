#pragma once
#include <GL/glew.h>
#include "graphics/Texture.hpp" 

class Mesh{
private:
    GLuint VAO, VBO, IBO, indexCount;
    Texture* texture;
public:
    Mesh();
    void createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void setTexture(Texture* texture);
    void renderMesh();
    void clearMesh();
    void createTriangle();
    ~Mesh();
};


