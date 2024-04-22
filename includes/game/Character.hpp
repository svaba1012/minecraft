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

enum UI_SUBTYPE{
    CHARACTER_UI_SUBTYPE,
    CRAFTING_UI_SUBTYPE,
    COOKING_UI_SUBTYPE,
    CHEST_UI_SUBTYPE
};

class Inventory;

class Item;

class Block;

class Character: public GameObject{
    
public:
    static Character* instance;

    float health;
    float armor;
    float hunger;
    float expLvl;

    bool isControllable = true;

    bool isInventoryUIOpen = false;
    int inventoryUISubtype = CHARACTER_UI_SUBTYPE;

    void openUI();
    void openUI(int type);

    float getArmor();

    Inventory* inventory;
    Inventory* protectionInventory;
    int selectedItemId = 0;

    Block* accessedBlock = NULL;

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


