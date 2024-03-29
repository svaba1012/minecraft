#pragma once
#include "game/GameObject.hpp"
#include "game/BlockType.hpp"
#include "minecraft/Item.hpp"
#include "game/OpenWorldScene.hpp"

#define DESTROY_STATES_NUM 10

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
    

    Block(Scene* scene);
    Block(Scene* scene, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);

    void setType(string type, string variant);

    void toogleOutline();
    void render(GLuint uniformModel, GLfloat deltaTime);


    

    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    ~Block();

protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);

};


