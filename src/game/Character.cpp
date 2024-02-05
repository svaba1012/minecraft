
#include "game/Character.hpp"


Character::Character(Scene *scene):GameObject(scene){
    this->axisOrientedBoundBoxMaxDot = glm::vec3(0.5, 0.5, 0.5);
    this->axisOrientedBoundBoxMinDot = glm::vec3(-0.5, -0.5, -0.5);
    this->isStatic = false;
    this->isCollideable = true;
}

Character::~Character(){
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

void Character::keyControl(bool* keys, GLfloat deltaTime){
    this->movementSpeed = 0.0;
    bool isMoving = false;
    glm::vec3 movementDirection = glm::vec3(0.0, 0.0, 0.0);
    if(keys[GLFW_KEY_W]){
        isMoving = true;
        movementDirection += this->front;
    }
    if(keys[GLFW_KEY_S]){
        isMoving = !isMoving;
        movementDirection -= this->front;
    }
    if(keys[GLFW_KEY_D] && !keys[GLFW_KEY_A]){
        isMoving = true;
        movementDirection += this->right;
    }
    if(keys[GLFW_KEY_A] && !keys[GLFW_KEY_D]){
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

