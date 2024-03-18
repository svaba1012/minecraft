#pragma once
#include "game/OpenWorldScene.hpp"
#include "game/GameObject.hpp"
#include "gameLib/inventory/Inventory.hpp"
#include "minecraft/Item.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"

#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>

using namespace std;

class Inventory;

class Item;

class Character: public GameObject{
    
public:
    static Character* instance;

    Inventory* inventory;
    Character(Scene *scene);
    void toogleOutline();
    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    void collectItem(Item* item);

    ~Character();
protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);
};


