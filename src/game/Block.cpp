#include "game/Block.hpp"

vector<Mesh*> Block::blockMeshes;
vector<Mesh*> Block::blockMeshesWithBorders;
Texture* Block::destroyStatesTextures[10];


Block::Block(Scene* scene):GameObject(scene){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->circleInscribedR = 0.5;
    this->circleOutscribedR = 1.73 / 2;
    this->isStatic = true;
    this->isCollideable = true;
    this->health = 1.0;
}

Block::Block(Scene* scene,glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale):GameObject(scene, pos,rotation, scale){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->circleInscribedR = 0.5;
    this->circleOutscribedR = 1.73 / 2;
    this->isStatic = true;
    this->isCollideable = true;
    this->health = 1.0;
}

void Block::setType(string type, string variant){
    this->typeName = type;
    this->variantName = variant;
    this->type = BlockType::getTypeByName(type);
    this->type->loadData();
}

void Block::setType(string variant){
    this->typeName = variant;
    this->variantName = variant;
    this->type = BlockType::getTypeByVariantName(variant);
    this->type->loadData();
}

void Block::onInteracted(GameObject* interactedBy, int type, GLfloat deltaTime){

    if(type == 0){ //hit
        this->health -= deltaTime / this->type->hardness;
        // change texture to destroy_stage_0.png ....
        // printf("HITTING %f\n", deltaTime);
        for(int i = 0; i < this->meshList->size(); i++){
            int destroyStage = (int)((1.0 - this->health) * DESTROY_STATES_NUM);
            //printf("%d\n", destroyStage);
            (*this->meshList)[i]->setOverlayTexture(Block::destroyStatesTextures[destroyStage]);
        }
        if(health <= 0.0){
            Item* dropItem = new Item(this->scene, this->pos, glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 1.0, 1.0));
            const float velocityIntensity = 2.0;
            const float angle1 = rand();
            const float angle2 = rand();
            dropItem->velocity = velocityIntensity * glm::vec3(sin(angle1)*cos(angle2), sin(angle1)*sin(angle2), cos(angle1));
            cout << "NAME"  << this->variantName << endl;
            dropItem->setTypeByName(this->variantName);
            // dropItem->setTypeById("dirt");
            dropItem->generateMeshes();
            ((OpenWorldScene*)this->scene)->addNewCollectable(dropItem);
        }
    }else if(type == 1){ // access, right click
        //check if type is accessible

        //suppose that user is interacting
        if(interactedBy == Character::instance){
            int selectedItemId = Character::instance->selectedItemId;
            MinecraftInventoryItem* itemInteractingWith = (MinecraftInventoryItem*)Character::instance->inventory->itemSlots[selectedItemId];
            if(itemInteractingWith == NULL){
                return;
            }

            if(itemInteractingWith->itemType->isBlock){
                //placing blocks
                // printf("Implement placing\n");

                int side = getSideAimingAt(((OpenWorldScene*)this->scene)->camera);
                
                printf("Placing block on %d side\n", side);

                float sign = side < 3 ? -1.0 : 1.0;

                glm::vec3 sideVec = sign * glm::vec3( \
                    side % 3 == 0, side % 3 == 1, side % 3 == 2);

                Block* block = new Block(this->scene, this->pos + sideVec, glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
                
                cout << itemInteractingWith->itemType->name << endl;

                block->setType(itemInteractingWith->itemType->name);
                block->generateMeshes();
                ((OpenWorldScene*)this->scene)->addNewGameObject(block);
                int count = itemInteractingWith->getCount();
                
                if(count == 1){
                    delete itemInteractingWith;
                    Character::instance->inventory->setInventoryItem(NULL, selectedItemId);
                    // itemInteractingWith = (MinecraftInventoryItem*) NULL;
                }else{
                    itemInteractingWith->setCount(--count);
                }
                
            }
        }


    }else if(type == 2){
        this->health = 1.0;
        for(int i = 0; i < this->meshList->size(); i++){
            (*this->meshList)[i]->setOverlayTexture((Texture*) NULL);
        }
        // printf("STOP HITTING\n");
    }

    
}


void Block::generateMeshWithBorders(GLfloat borderWidth, vector<Mesh*> *listMesh){
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

    GLfloat texHigh = 1.0 + borderWidth;
    GLfloat texLow = -borderWidth;

    GLfloat vertices[] = {
    //  x       y       z       u    v 
        0.0,   0.0,   0.0,    texLow, texLow,
        1.0,   0.0,   0.0,    texHigh, texLow,
        0.0,   0.0,   1.0,    texHigh, texLow,
        1.0,   0.0,   1.0,    texLow, texLow,
        0.0,   1.0,   0.0,    texLow, texHigh,
        1.0,   1.0,   0.0,    texHigh, texHigh,
        0.0,   1.0,   1.0,    texHigh, texHigh,
        1.0,   1.0,   1.0,    texLow, texHigh
    };

    GLfloat bottomVertices[] = {
    //  x       y       z       u    v 
        0.0,   0.0,   0.0,    texLow, texLow,
        1.0,   0.0,   0.0,    texHigh, texLow,
        0.0,   0.0,   1.0,    texLow, texHigh,
        1.0,   0.0,   1.0,    texHigh, texHigh    
    };

    GLfloat topVertices[] = {
    //  x       y       z       u    v 
        0.0,   1.0,   0.0,    texLow, texLow,
        1.0,   1.0,   0.0,    texHigh, texLow,
        0.0,   1.0,   1.0,    texLow, texHigh,
        1.0,   1.0,   1.0,    texHigh, texHigh  
    };

    Texture *topText = this->type->getTexturesByVariantName(this->variantName, 0);

    Mesh* topMesh = new Mesh();
    topMesh->createMesh(topVertices, topIndices, 20, 6);
    topMesh->setTexture(topText);
    listMesh->push_back(topMesh);

    Texture *sideText = this->type->getTexturesByVariantName(this->variantName, 1);

    Mesh* sideMesh = new Mesh();
    sideMesh->createMesh(vertices, sideIndices, 40, 24);
    sideMesh->setTexture(sideText);
    listMesh->push_back(sideMesh);

    Texture *bottomText = this->type->getTexturesByVariantName(this->variantName, 2);

    Mesh* bottomMesh = new Mesh();
    bottomMesh->createMesh(bottomVertices, bottomIndices, 20, 6);
    bottomMesh->setTexture(bottomText);
    listMesh->push_back(bottomMesh);


}

void Block::generateMeshes(){
    if(Block::blockMeshes.size() > 0){
        this->meshList = &Block::blockMeshes;
        return;
    }

    

    for(int i = 0; i < DESTROY_STATES_NUM; i++){
        char fileName[100] = "./assets/extern_minecraft_assets/assets/minecraft/textures/block/destroy_stage_5.png";
        int len = sprintf(fileName, "./assets/extern_minecraft_assets/assets/minecraft/textures/block/destroy_stage_%d.png", i);
        fileName[len] = '\0';
        printf("%s\n", fileName);

        Block::destroyStatesTextures[i] = new Texture(fileName);
        Block::destroyStatesTextures[i]->loadTexture(1);
    }

    generateMeshWithBorders(0.0, &Block::blockMeshes);
    generateMeshWithBorders(0.02, &Block::blockMeshesWithBorders);

    this->meshList = &Block::blockMeshes;
}

void Block::render(GLuint uniformModel, GLfloat deltaTime){
    for(int i = 0; i < this->meshList->size(); i++){
        Texture *curTexture = this->type->getTexturesByVariantName(this->variantName, i);
        (*this->meshList)[i]->setTexture(curTexture);
        // (*this->meshList)[i]->setOverlayTexture(Block::destroyStatesTextures[0]);
    }
    update();
    glm::mat4 model = glm::mat4(1);
    // model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));
    model = glm::translate(model, this->pos);
    // model = glm::scale(model, this->scale);
    // model = glm::rotate(model, glm::radians(this->roll), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-this->yaw), glm::vec3(0.0, 1.0, 0.0));    
    // model = glm::rotate(model, glm::radians(this->pitch), glm::vec3(0.0, 0.0, 1.0));   
    
    model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));
     
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    
    for(int i = 0; i < this->meshList->size(); i++){
        (*meshList)[i]->renderMesh();
    }
}

void Block::toogleOutline(){
    if(this->meshList == &Block::blockMeshes){
        this->meshList = &Block::blockMeshesWithBorders;
    }else{
        this->meshList = &Block::blockMeshes;
    }
}

int Block::getSideAimingAt(Camera* camera){
    glm::vec3 aimLineDot = camera->position;
    glm::vec3 aimLineDirection = camera->front;
    
    aimLineDirection = glm::normalize(aimLineDirection);
    bool hasOneIntersection = false;

    float distance;
    glm::vec3 intersection;
    int side = -1;
    
    for(float dot = -0.5; dot <= 0.5; dot++){
        glm::vec3 planeDot = glm::vec3(dot, dot, dot) + this->pos;
        for(int unitVec = 0; unitVec < 3; unitVec++){
            glm::vec3 planeNormal = glm::vec3(unitVec == 0, unitVec == 1, unitVec == 2);
            
            float planeNormalCrossLineAim = glm::dot(aimLineDirection, planeNormal);
            if(planeNormalCrossLineAim == 0.0){
                continue;
            }
            float d = glm::dot(planeDot - aimLineDot, planeNormal) / planeNormalCrossLineAim;
            glm::vec3 tempIntersection = aimLineDot + d * aimLineDirection;
            
            glm::vec3 opositeDotSquareDiff = glm::vec3(unitVec != 0, unitVec != 1, unitVec != 2);
            // printf("Plane normal %f, %f, %f\n", opositeDotSquareDiff.x, opositeDotSquareDiff.y, opositeDotSquareDiff.z);
            glm::vec3 maxDotSquare;
            glm::vec3 minDotSquare; 
            if(dot > 0){
                maxDotSquare = planeDot;
                minDotSquare = planeDot - opositeDotSquareDiff; 
            }else{
                minDotSquare = planeDot;
                maxDotSquare = planeDot + opositeDotSquareDiff;
            }

             
            bool isWithinX =   (unitVec == 0) || \
            (tempIntersection.x >= minDotSquare.x && tempIntersection.x <= maxDotSquare.x);
            bool isWithinY =   (unitVec == 1) || \
            (tempIntersection.y >= minDotSquare.y && tempIntersection.y <= maxDotSquare.y);
            bool isWithinZ =   (unitVec == 2) || \
            (tempIntersection.z >= minDotSquare.z && tempIntersection.z <= maxDotSquare.z);

            if(isWithinX && isWithinY && isWithinZ){
                if(!hasOneIntersection){
                    intersection = tempIntersection;
                    hasOneIntersection = true;
                    side = (dot + 0.5) * 3 + unitVec;
                    distance = d;
                }else{
                    float oldLen = glm::length(aimLineDot - intersection);
                    float newLen = glm::length(aimLineDot - tempIntersection);
                    if(d < distance){
                        distance = d;
                        intersection = tempIntersection;
                        side = (dot + 0.5) * 3 + unitVec;
                    }
                }
                
            }
            
        }
    }
    return side;
}

bool Block::isAimedAt(Camera* camera){
    return getSideAimingAt(camera) > 0;    
}


void Block::keyControl(bool* keys, GLfloat deltaTime){

}

void Block:: mouseControl(GLfloat xChange, GLfloat yChange){

}

Block::~Block(){
    //printf("Implement item dropping\n");
}