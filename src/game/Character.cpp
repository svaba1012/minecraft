
#include "game/Character.hpp"


Character* Character::instance;

Character::Character(Scene *scene):GameObject(scene){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->isStatic = false;
    this->isCollideable = true;
    this->inventory = new Inventory(36);
    this->protectionInventory = new Inventory(5);
    this->health = 5.5;
    this->armor = 0.0;
    this->hunger = 10.0;
    this->expLvl = 15.5;
    Character::instance = this;
}

Character::~Character(){
}

void Character::openUI(){
    isControllable = !isControllable;
    isInventoryUIOpen = !isControllable;
    inventoryUISubtype = CHARACTER_UI_SUBTYPE;
}

void Character::openUI(int type){
    inventoryUISubtype = type;
    isControllable = false;
    isInventoryUIOpen = true;
}

// enum ARMOR_TYPES = {ARMOR_TYPE_HELMET, ARMOR_TYPE_CHESTPLATE, ARMOR_TYPE_LEGGINS, ARMOR_TYPE_BOOTS};
// enum ARMOR_MATERIAL = {LEATHER, GOLD, CHAINMAIL, IRON, DIAMOND, NETHERITE, TURTLE};
#define ARMOR_MATERIAL_NUM 7
#define ARMOR_TYPE_NUM 4

char ARMOR_MATERIAL_STRINGS[ARMOR_MATERIAL_NUM][20] = {"leather", "golden",
"chainmail", "iron", "diamond", "netherite", "turtle"};

int ARMOR_PTS[ARMOR_MATERIAL_NUM][ARMOR_TYPE_NUM] = { 
            //helmet, chestplate, leggins, boots
                        {1, 3, 2, 1},   //leather
                        {2, 5, 3, 1},   //gold
                        {2, 5, 4, 1},   //chainmail
                        {2, 6, 5, 2},   //iron
                        {3, 8, 6, 3},   //diamond
                        {3, 8, 6, 3},   //netherite
                        {2, 0, 0, 0},   //turtle
};

float Character::getArmor(){
    float armorSum = 0.0;
    for(int i = 0; i < protectionInventory->itemSlots.size() - 1; i++){
        MinecraftInventoryItem* minecraftItem = (MinecraftInventoryItem*) protectionInventory->itemSlots[i];
        if(minecraftItem == NULL){
            continue;
        }
        string itemIdStr = minecraftItem->itemType->getId();
        int armorMaterialIndex = -1;
        for(int j = 0; j < ARMOR_MATERIAL_NUM; j++){
            bool match = itemIdStr.find(ARMOR_MATERIAL_STRINGS[j]) != std::string::npos;
            if(match){
                armorMaterialIndex = j;
                break;
            }
        }
        armorSum += ARMOR_PTS[armorMaterialIndex][i] / 2.0;
    }
    return armorSum;
}


void Character::toogleOutline(){

}


void Character::generateMeshes(){
    this->meshList = new vector<Mesh*>();

     unsigned int sideIndices[] = {
        0, 1, 4, 1, 4, 5,
        0, 2, 4, 2, 4, 6,
        1, 3, 5, 3, 5, 7,
        2, 3, 6, 3, 6, 7,    
    };

    unsigned int bottomIndices[] = {
        0, 1, 2, 1, 2, 3,    
    };

    unsigned int topIndices[] = {
        0, 1, 2, 1, 2, 3,    
    };

    GLfloat vertices[] = {
    //  x       y       z       u    v 
        0.0,   0.0,   0.0,    0.0, 0.0,
        1.0,   0.0,   0.0,    1.0, 0.0,
        0.0,   0.0,   1.0,    1.0, 0.0,
        1.0,   0.0,   1.0,    0.0, 0.0,
        0.0,   1.0,   0.0,    0.0, 1.0,
        1.0,   1.0,   0.0,    1.0, 1.0,
        0.0,   1.0,   1.0,    1.0, 1.0,
        1.0,   1.0,   1.0,    0.0, 1.0
    };

    GLfloat bottomVertices[] = {
    //  x       y       z       u    v 
        0.0,   0.0,   0.0,    0.0, 0.0,
        1.0,   0.0,   0.0,    1.0, 0.0,
        0.0,   0.0,   1.0,    0.0, 1.0,
        1.0,   0.0,   1.0,    1.0, 1.0    
    };

    GLfloat topVertices[] = {
    //  x       y       z       u    v 
        0.0,   1.0,   0.0,    -0.0, -0.0,
        1.0,   1.0,   0.0,    1.0, -0.0,
        0.0,   1.0,   1.0,    -0.0, 1.0,
        1.0,   1.0,   1.0,    1.0, 1.0    
    };

    Texture *dirtText = new Texture("./assets/textures/brick.png");

    Mesh* sideMesh = new Mesh();
    sideMesh->createMesh(vertices, sideIndices, 40, 24);
    sideMesh->setTexture(dirtText);
    this->meshList->push_back(sideMesh);

    Mesh* bottomMesh = new Mesh();
    bottomMesh->createMesh(bottomVertices, bottomIndices, 20, 6);
    bottomMesh->setTexture(dirtText);
    meshList->push_back(bottomMesh);

    Mesh* topMesh = new Mesh();
    topMesh->createMesh(topVertices, topIndices, 20, 6);
    topMesh->setTexture(dirtText);
    meshList->push_back(topMesh);
}

void Character::onKeys(int key, int code, int action, int mode){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_E){
            openUI();
        }else if(key == GLFW_KEY_1){
            this->selectedItemId = 0;
        }else if(key == GLFW_KEY_2){
            this->selectedItemId = 1;
        }else if(key == GLFW_KEY_3){
            this->selectedItemId = 2;
        }else if(key == GLFW_KEY_4){
            this->selectedItemId = 3;
        }else if(key == GLFW_KEY_5){
            this->selectedItemId = 4;
        }else if(key == GLFW_KEY_6){
            this->selectedItemId = 5;
        }else if(key == GLFW_KEY_7){
            this->selectedItemId = 6;
        }else if(key == GLFW_KEY_8){
            this->selectedItemId = 7;
        }else if(key == GLFW_KEY_9){
            this->selectedItemId = 8;
        }
    }
}

void Character::keyControl(bool* keys, GLfloat deltaTime){
    //!SHOULD NOT BE HERE
        // if(hunger > 0.0){
        //     hunger -= deltaTime / 10;
        // }
        if(hunger < 0.5){
            health -= deltaTime;
        }
        if(hunger == 10.0 && health < 10.0){
            health += deltaTime;
        }


    //!

    this->movementSpeed = 0.0;
    bool isMoving = false;
    glm::vec3 movementDirection = glm::vec3(0.0, 0.0, 0.0);
    if(keys[GLFW_KEY_W] && isControllable){
        isMoving = true;
        movementDirection += this->front;
    }
    if(keys[GLFW_KEY_S] && isControllable){
        isMoving = !isMoving;
        movementDirection -= this->front;
    }
    if(keys[GLFW_KEY_D] && !keys[GLFW_KEY_A] && isControllable){
        isMoving = true;
        movementDirection += this->right;
    }
    if(keys[GLFW_KEY_A] && !keys[GLFW_KEY_D] && isControllable){
        isMoving = true;
        movementDirection -= this->right;
    }
    
    if(isMoving){
        this->movementSpeed = 4.0;
        this->velocity = this->movementSpeed * glm::normalize(movementDirection);
        // this->applyMovement(glm::normalize(movementDirection), deltaTime);
    }else{
        this->velocity = glm::vec3(0.0, 0.0, 0.0);
    }
    
}

void Character:: mouseControl(GLfloat xChange, GLfloat yChange){
    if(isControllable){
        this->yawSpeed = 0.1;
        this->pitchSpeed = 0.1;
        // printf("Character yaw: %f\n",  yaw);

        // glm::radians(yaw)
        this->yaw += xChange * this->yawSpeed;
        this->pitch -= yChange * this->pitchSpeed;

        if(this->pitch > 89.0){
            this->pitch = 89.0;
        }else if( this->pitch < -89.0){
            this->pitch = -89.0;
        }
        this->update();
    }
    
}

void Character::collectItem(Item* item){
    MinecraftInventoryItem* invItem = new MinecraftInventoryItem(item->type);
    if(inventory->pushItemInInventory(invItem) >= 0){
        ((OpenWorldScene*)this->scene)->removeCollectable(item);
    }
}


void Character::onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime){

    
}

