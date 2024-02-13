#pragma once
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader{
private:
    GLuint program;

    vector<GLuint> shaderPrograms;
    
    /* data */
public:
    Shader();
    bool addShader(char* filename, GLenum shaderType);
    void compileShaders();

    GLuint getUniformLocation(char* uniformName);
    GLuint getAttribLocation(char* attrName);
    GLuint getModelLocation();
    GLuint getPerspectiveLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();


    void useShader();

    
    ~Shader();
};



