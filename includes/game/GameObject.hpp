
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"


#include "graphics/Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
// #include "Scene.hpp"


using namespace std;

class Scene;

class GameObject{
protected:
    Scene* scene;

    vector<Mesh*> *meshList;
    // vector<Texture*> textureList; 
    

    glm::vec3 axisOrientedBoundBoxMaxDot;
    glm::vec3 axisOrientedBoundBoxMinDot;

    virtual void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime) = 0;

public:
    bool isStatic;
    bool isCollideable;
    /* data */
    glm::vec3 pos;
    glm::vec3 scale;
    GLfloat roll;
    GLfloat yaw;
    GLfloat pitch;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 velocity;
    GLfloat movementDirection;
    GLfloat movementSpeed;
    GLfloat rollSpeed;
    GLfloat yawSpeed = 0.0;
    GLfloat pitchSpeed = 0.0;
    GLfloat scaleSpeed;
    GLfloat circleInscribedR;
    GLfloat circleOutscribedR;

    float health;

    GameObject(Scene* scene);
    virtual void generateMeshes() = 0;
    virtual void keyControl(bool* keys, GLfloat deltaTime) = 0;
    virtual void mouseControl(GLfloat xChange, GLfloat yChange) = 0;
    virtual void toogleOutline() = 0;
    
    void draw(GLfloat scale, GLuint uniformModel);

    virtual void render(GLuint uniformModel, GLfloat deltaTime);
    void update();

    void applyMovement(GLfloat deltaTime);
    bool isCollidingWith(GameObject* anotherObject, GLfloat deltaTime);

    void shit_func(GLfloat deltaTime);

    GameObject* getObjectAimingAt();

    void interactWithObject(GameObject* go, int type, GLfloat deltaTime);

    
    
    
    GameObject(Scene* scene, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);
    ~GameObject();
};


