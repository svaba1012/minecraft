#pragma once
#include "game/GameObject.hpp"
#include "game/BlockType.hpp"
#include "minecraft/Item.hpp"
#include "game/OpenWorldScene.hpp"
#include "gameLib/inventory/InventoryItem.hpp"

#define DESTROY_STATES_NUM 10

class BlockType;

class Block: public GameObject{
private:
    /* data */
    static vector<Mesh*> blockMeshesWithBorders;
    static Texture* destroyStatesTextures[10];

    void generateMeshWithBorders(GLfloat borderWidth, vector<Mesh*> *listMesh);


     
public:
    static vector<Mesh*> blockMeshes;
    BlockType* type;
    string typeName;
    string variantName;

    //!BLOCK STATE - In future remove from here
    
    // Inventory* itemContent = NULL;
    double maxFuelTime = 0.0;
    double fuelTimeLeft = 0.0;
    double smeltTimeLeft = 0.0;
    InventoryItem* fuelItem = NULL;
    InventoryItem* cookItem = NULL;
    InventoryItem* smeltedItem = NULL;
    ItemType* smeltResult = NULL;

    bool canItSmelt = true;
    bool hasSomethingSmelting = false;
    //! use later
    ItemType* typeToSmeltTo = NULL;
    
    //!
    

    Block(Scene* scene);
    Block(Scene* scene, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);

    void setType(string type, string variant);
    void setType(string variant);

    bool hasState();
    void updateState(double deltaTime);

    void toogleOutline();
    void render(GLuint uniformModel, GLfloat deltaTime);

    
    int getSideAimingAt(Camera* camera);
    bool isAimedAt(Camera* camera);

    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    ~Block();

protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);

};


