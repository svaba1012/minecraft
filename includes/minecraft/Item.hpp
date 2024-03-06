#pragma once
#include "game/GameObject.hpp"
#include "minecraft/ItemType.hpp"
#include "game/Block.hpp"

class Item: public GameObject{
private:
    /* data */
    // ? maybe unnecessery
    static vector<Mesh*> squareMeshes;
    void generateMeshWithBorders(GLfloat borderWidth, vector<Mesh*> *listMesh);

     
public:
    ItemType* type;
    string typeName;
    string typeId;

    // ? maybe no
    string variantName;

    Item(Scene* scene);
    Item(Scene* scene, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);

    void setTypeByName(string type);
    void setTypeById(string type);

    void render(GLuint uniformModel, GLfloat deltaTime);

    void generateMeshes();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    void toogleOutline();

    ~Item();
protected:
    void onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime);
};


