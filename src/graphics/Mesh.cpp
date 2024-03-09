#include "graphics/Mesh.hpp"

Mesh::Mesh(/* args */){
    this->VAO = 0;
    this->VBO = 0;
    this->IBO = 0;
    this->texture = NULL;
    this->overlayTexture = NULL;
}



void Mesh::createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices){

    this->indexCount = numOfIndices;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::setTexture(Texture* texture){
    this->texture = texture;
    // this->texture->loadTexture();
}

void Mesh::setOverlayTexture(Texture* texture){
    this->overlayTexture = texture;
}

void Mesh::renderMesh(){
    if(this->texture){
        this->texture->useTexture();
    }
    if(this->overlayTexture){
        glUniform1i(glGetUniformLocation(4, "hasOverlayTexture"), 1);
        this->overlayTexture->useTexture();
    }else{
        glUniform1i(glGetUniformLocation(4, "hasOverlayTexture"), 0);
    }
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->IBO);
    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::clearMesh(){

}

Mesh::~Mesh(){
    this->clearMesh();
}