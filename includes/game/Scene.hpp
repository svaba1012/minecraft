
#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "graphics/Shader.hpp"
#include "Camera.hpp"
#include "game/Character.hpp"
#include "Light.hpp"
#include "game/Block.hpp"
#include "game/GameObject.hpp"

using namespace std;

class Character;

class Scene{
private:
    Shader* shader;
    vector<Shader> shaderList;
    GLuint uniformModel, uniformPerspective, uniformView, uniformLightColor, uniformLightIntensity;
    GLfloat deltaTime, nowTime, lastTime = 0.0;
    
    Light light;
    
    bool* keys;
    bool* mouseButtons;

    GameObject* objectAttachedCamera;
    glm::vec3 cameraOffset;
    vector<GameObject*> gameObjects;
    vector<GameObject*> userControllableGameObjects;
public:
    Camera* camera;
const GLint WIDTH = 800, HEIGHT = 600;
    GLfloat xChange;
    GLfloat yChange;
    
    Scene();
    void init(bool* keys, bool* mouseButtons);
    void render();

    void setCamera();

    bool* getKeys();

    

    GLfloat getXChange();

    GLfloat getYChange();

    glm::mat4 getViewMatrix();
    
    ~Scene();
};