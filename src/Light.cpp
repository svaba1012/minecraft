#include <./Light.hpp>

Light::Light(/* args */){
    this->color = glm::vec3(1.0, 1.0, 1.0);
    this->ambientIntensity = 1.0;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity){
    this->color = glm::vec3(red, green, blue);
    this->ambientIntensity = intensity;
}

void Light::useLight(GLuint colorLocation, GLuint intensityLocation){    
    glUniform3f(colorLocation, color[0], color[1], color[2]);
    
    glUniform1f(intensityLocation, this->ambientIntensity);
}

Light::~Light(){
}