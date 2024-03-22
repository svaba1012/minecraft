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

    float health;
    float armor;
    float hunger;
    float expLvl;

    bool isControllable = true;

    void toogleControls();

    float getArmor();

    Inventory* inventory;
    Inventory* protectionInventory;
    Character(Scene *scene);
    void toogleOutline();
    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    void collectItem(Item* item);
    void onKeys(int key, int code, int action, int mode);

    ~Character();
protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);
};


