#pragma once
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader{
private:
    GLuint VAO, VBO, program;
    
    /* data */
public:
    Shader(/* args */);
    void createTriangle(GLfloat* vertices, GLuint verticesLen);
    bool addShader(char* filename, GLenum shaderType);
    void compileShaders();

    GLuint getModelLocation();
    GLuint getPerspectiveLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();


    void useShader();

    
    ~Shader();
};



