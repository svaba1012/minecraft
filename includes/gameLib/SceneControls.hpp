
#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "graphics/Shader.hpp"
#include "gameLib/ui/GameUI2D.hpp"
#include "Window.hpp"
#include "gameLib/inventory/InventoryUI.hpp"


using namespace std;

class Character;

class SceneControls: public Scene{
private:
    Shader* shader;
    vector<Shader> shaderList;
    GLuint uniformModel, uniformPerspective, uniformView, uniformLightColor, uniformLightIntensity;
    GLfloat deltaTime, nowTime, lastTime = 0.0;
    
    bool* keys;
    bool* mouseButtons;

    vector<GameUI2D*> gameUIs;
    GameUI2D* activeUI;
    const GLint WIDTH = 800, HEIGHT = 600;

public:
    void addNewGameUIObject(GameUI2D* gUIO);
    bool isInventoryOpen = false;
    // GLfloat xChange;
    // GLfloat yChange;
    
    // double time;

    SceneControls();
    void init(bool* keys, bool* mouseButtons);
    void render();
    
    bool* getKeys();

    GLfloat getXChange();

    GLfloat getYChange();

    void setCamera();
    glm::mat4 getViewMatrix();

    void onKeys(int key, int code, int action, int mode);
    
    ~SceneControls();
};