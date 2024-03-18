
#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>



using namespace std;



class Scene{

    

public:

    GLfloat xChange;
    GLfloat yChange;
    double time;
    virtual void init(bool* keys, bool* mouseButtons) = 0;
    virtual void render() = 0;


    virtual bool* getKeys() = 0;

    virtual GLfloat getXChange() = 0;

    virtual GLfloat getYChange() = 0;

    virtual void setCamera() = 0;
    virtual glm::mat4 getViewMatrix() = 0;

    virtual void onKeys(int key, int code, int action, int mode) = 0;
};