#include "minecraft/Item.hpp"

vector<Mesh*> Item::squareMeshes;

Item::Item(Scene* scene):GameObject(scene){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->circleInscribedR = 0.5;
    this->circleOutscribedR = 1.73 / 2;
    this->isStatic = true;
    this->isCollideable = true;
}

Item::Item(Scene* scene,glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale):GameObject(scene, pos,rotation, scale){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->circleInscribedR = 0.5;
    this->circleOutscribedR = 1.41 / 2;
    this->isStatic = true;
    this->isCollideable = true;

    this->scale = glm::vec3(0.4, 0.4, 0.4);
    this->yawSpeed = 50.1;
}

void Item::setTypeByName(string type){
    this->typeName = type;
    this->type = ItemType::getTypeByName(type);
    this->type->loadData();
}

void Item::setTypeById(string type){
    this->typeId = type;
    this->type = ItemType::getTypeById(type);
    this->type->loadData();
}

void Item::onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime){

}


void Item::generateMeshes(){
    if(this->type->isBlock && Block::blockMeshes.size() > 0){
        this->meshList = &Block::blockMeshes;
        return;
    }else if (!this->type->isBlock && Item::squareMeshes.size() > 0){
        this->meshList = &Item::squareMeshes;
    }else if (!this->type->isBlock){
        GLfloat texHigh = 1.0;
        GLfloat texLow = 0.0;

        GLfloat vertices[] = {
        //  x       y       z       u    v 
            0.0,   0.0,   0.0,    texHigh, texHigh,
            1.0,   0.0,   0.0,    texLow, texHigh,
            0.0,   1.0,   0.0,    texHigh, texLow,
            1.0,   1.0,   0.0,    texLow, texLow  
        };

        unsigned int indices[] = {
        0, 1, 2, 1, 2, 3,    
        };

        //! load text from itemType 
        Texture *text = this->type->getTextures(0);

        Mesh* mesh = new Mesh();
        mesh->createMesh(vertices, indices, 20, 6);
        mesh->setTexture(text);
        Item::squareMeshes.push_back(mesh);
        this->meshList = &Item::squareMeshes;
    }else{
        printf("Error generating mesh for item\n");
    }
}

void Item::render(GLuint uniformModel, GLfloat deltaTime){
    for(int i = 0; i < this->meshList->size(); i++){
        Texture *curTexture = this->type->getTextures(i);
        (*this->meshList)[i]->setTexture(curTexture);
    }
    this->yaw += this->yawSpeed * deltaTime;
    update();
    glm::mat4 model = glm::mat4(1);
    // model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));
    model = glm::translate(model, this->pos);
    model = glm::scale(model, this->scale);
    // model = glm::rotate(model, glm::radians(this->roll), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-this->yaw), glm::vec3(0.0, 1.0, 0.0));    
    // model = glm::rotate(model, glm::radians(this->pitch), glm::vec3(0.0, 0.0, 1.0));   
    
    model = glm::translate(model, glm::vec3(-0.5, -0.5, this->type->isBlock ? -0.5 : 0.0));
     
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    
    for(int i = 0; i < this->meshList->size(); i++){
        (*meshList)[i]->renderMesh();
    }
    // aaa
}

void Item::keyControl(bool* keys, GLfloat deltaTime){

}
void Item::mouseControl(GLfloat xChange, GLfloat yChange){

}
void Item::toogleOutline(){
    
}


Item::~Item(){

}