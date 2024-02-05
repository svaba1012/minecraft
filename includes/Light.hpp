#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>


class Light{
private:
    glm::vec3 color;
    GLfloat ambientIntensity;
public:
    Light(/* args */);
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity);
    void useLight(GLuint colorLocation, GLuint intensityLocation);
    ~Light();
};


