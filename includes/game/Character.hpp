#pragma once
#include "game/Scene.hpp"
#include "game/GameObject.hpp"

#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>

using namespace std;


class Character: public GameObject{
    
public:
    Character(Scene *scene);
    void toogleOutline();
    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    ~Character();
protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);
};


