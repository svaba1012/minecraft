#include "game/GameObject.hpp"
#include "gameLib/Scene.hpp"
#include "game/OpenWorldScene.hpp"



GameObject::GameObject(Scene* scene){
    this->scene = scene;
    this->pos = glm::vec3(0.0, 0.0, 0.0);
    this->roll = 0.0;
    this->yaw = 0.0;
    this->pitch = 0.0;
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->scaleSpeed = 0.0;
    this->movementSpeed = 0.0;
    this->rollSpeed = 0.0;
    this->yawSpeed = 0.0;
    this->pitchSpeed = 0.0;
    this->velocity = glm::vec3(0.0, 0.0, 0.0);
    this->acceleration = glm::vec3(0.0, 0.0, 0.0);
    this->isAffectedByGravity = false;
}

GameObject::GameObject(Scene* scene, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale){
    this->scene = scene;
    this->pos = pos;
    this->roll = rotation[0];
    this->yaw = rotation[1];
    this->pitch = rotation[2];
    this->scale = scale;
    this->velocity = glm::vec3(0.0, 0.0, 0.0);
    this->acceleration = glm::vec3(0.0, 0.0, 0.0);
    this->update();
    this->isAffectedByGravity = false;
}

void GameObject::onKeys(int key, int code, int action, int mode){

}


void GameObject::setAffectedByGravity(bool isAffected){
    //non moveable objects can't be affected be gravity
    this->isAffectedByGravity = isAffected;
    if(isAffected){
        this->acceleration += glm::vec3(0.0, -((OpenWorldScene*)scene)->gravityIntensity, 0.0);
        //this->velocity +=  glm::vec3(0.0, -scene->gravityIntensity, 0.0);
    }
}

void GameObject::shit_func(GLfloat deltaTime){
    this->pos += this->front * this->movementSpeed * deltaTime;
    this->scale += glm::normalize(this->scale) * this->scaleSpeed * deltaTime;
    this->roll += this->rollSpeed * deltaTime;
    this->yaw += this->yawSpeed * deltaTime;
    this->pitch += this->pitchSpeed * deltaTime;
}

void GameObject::applyMovement(GLfloat deltaTime){
    this->velocity += deltaTime * this->acceleration;
    this->pos += deltaTime * this->velocity;
}

int GameObject::isCollidingWith(GameObject* anotherObject, GLfloat deltaTime){
    glm::vec3 thisPotentialPos = this->pos + deltaTime * this->velocity;
    glm::vec3 otherPotentialPos = anotherObject->pos + deltaTime * anotherObject->velocity;

    glm::vec3 axisOrientedBoundBoxMaxDot;
    glm::vec3 axisOrientedBoundBoxMinDot;

    glm::vec3 thisMaxDot = this->axisOrientedBoundBoxMaxDot + thisPotentialPos;
    glm::vec3 thisMinDot = this->axisOrientedBoundBoxMinDot + thisPotentialPos;
    glm::vec3 otherMaxDot = anotherObject->axisOrientedBoundBoxMaxDot + otherPotentialPos;
    glm::vec3 otherMinDot = anotherObject->axisOrientedBoundBoxMinDot + otherPotentialPos;
    
    if(thisMinDot.x > otherMaxDot.x){
        return 0;
    }
    if(thisMaxDot.x < otherMinDot.x){
        return 0;
    }
    if(thisMinDot.y > otherMaxDot.y){
        return 0;
    }
    if(thisMaxDot.y < otherMinDot.y){
        return 0;
    }
    if(thisMinDot.z > otherMaxDot.z){
        return 0;
    }
    if(thisMaxDot.z < otherMinDot.z){
        return 0;
    }

    //check is x axis collision
    thisPotentialPos = this->pos;
    thisPotentialPos.x = this->pos.x + deltaTime * this->velocity.x;

    thisMaxDot = this->axisOrientedBoundBoxMaxDot + thisPotentialPos;
    thisMinDot = this->axisOrientedBoundBoxMinDot + thisPotentialPos;
    bool isAxisCollision = true;
    if(thisMinDot.x > otherMaxDot.x){
        isAxisCollision = false;
    }else if(thisMaxDot.x < otherMinDot.x){
        isAxisCollision = false;
    }else if(thisMinDot.y > otherMaxDot.y){
        isAxisCollision = false;
    }else if(thisMaxDot.y < otherMinDot.y){
        isAxisCollision = false;
    }else if(thisMinDot.z > otherMaxDot.z){
        isAxisCollision = false;
    }else if(thisMaxDot.z < otherMinDot.z){
        isAxisCollision = false;
    }
    if(isAxisCollision){
        printf("Collision x detected\n");
        return 1; //1
    }

//check is y axis collision
    thisPotentialPos = this->pos;
    thisPotentialPos.y = this->pos.y + deltaTime * this->velocity.y;

    thisMaxDot = this->axisOrientedBoundBoxMaxDot + thisPotentialPos;
    thisMinDot = this->axisOrientedBoundBoxMinDot + thisPotentialPos;
    isAxisCollision = true;
    if(thisMinDot.x > otherMaxDot.x){
        isAxisCollision = false;
    }else if(thisMaxDot.x < otherMinDot.x){
        isAxisCollision = false;
    }else if(thisMinDot.y > otherMaxDot.y){
        isAxisCollision = false;
    }else if(thisMaxDot.y < otherMinDot.y){
        isAxisCollision = false;
    }else if(thisMinDot.z > otherMaxDot.z){
        isAxisCollision = false;
    }else if(thisMaxDot.z < otherMinDot.z){
        isAxisCollision = false;
    }
    if(isAxisCollision){
        printf("Collision y detected\n");
        return 2; //2
    }

    //check is y axis collision
    thisPotentialPos = this->pos;
    thisPotentialPos.z = this->pos.z + deltaTime * this->velocity.z;

    thisMaxDot = this->axisOrientedBoundBoxMaxDot + thisPotentialPos;
    thisMinDot = this->axisOrientedBoundBoxMinDot + thisPotentialPos;
    isAxisCollision = true;
    if(thisMinDot.x > otherMaxDot.x){
        isAxisCollision = false;
    }else if(thisMaxDot.x < otherMinDot.x){
        isAxisCollision = false;
    }else if(thisMinDot.y > otherMaxDot.y){
        isAxisCollision = false;
    }else if(thisMaxDot.y < otherMinDot.y){
        isAxisCollision = false;
    }else if(thisMinDot.z > otherMaxDot.z){
        isAxisCollision = false;
    }else if(thisMaxDot.z < otherMinDot.z){
        isAxisCollision = false;
    }
    if(isAxisCollision){
        printf("Collision z detected\n");
        return 3; //3
    }


    
    return true;
}

GameObject* GameObject::getObjectAimingAt(){
    // for(int i = 0; i < this->scene->gameObjects.size(); i++){
    //     GameObject* curGo = this->scene->gameObjects[i];
    //     glm::vec3 distanceVec = curGo->pos - this->pos;
    //     GLfloat sinAngle = glm::length(glm::cross(this->front, distanceVec));
    //     printf("Spec value: %f\n", sinAngle);
    // }
    return this;
}


void GameObject::update(){


    
    
    this->front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    this->front.y = sin(glm::radians(pitch));
    this->front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->front = glm::normalize(this->front);

    this->right.x = cos(glm::radians(roll)) * cos(glm::radians(yaw) + 3.14 / 2);
    this->right.y = sin(glm::radians(roll));
    this->right.z = cos(glm::radians(roll)) * sin(glm::radians(yaw) + 3.14 / 2);
    this->right = glm::normalize(this->right);

    // this->right = glm::normalize(glm::cross(this->front, glm::vec3(0.0, 1.0, 0.0)));
    // this->up = glm::normalize(glm::cross(this->right, this->front));

    // this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void GameObject::render(GLuint uniformModel, GLfloat deltaTime){
    // this->yaw += this->yawSpeed * deltaTime;
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

// void GameObject::draw(GLfloat scale, GLuint uniformModel){
    
//     glm::mat4 model = glm::mat4(1);
//     // model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));
//     model = glm::translate(model, this->pos);
//     model = glm::scale(model, glm::vec3(scale, scale, scale));
//     // model = glm::rotate(model, glm::radians(this->roll), glm::vec3(1.0, 0.0, 0.0));
//     model = glm::rotate(model, glm::radians(-this->yaw), glm::vec3(0.0, 1.0, 0.0));    
//     // model = glm::rotate(model, glm::radians(this->pitch), glm::vec3(0.0, 0.0, 1.0));   
    
//     model = glm::translate(model, glm::vec3(-0.5, -0.5, -0.5));
     
//     glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    
//     for(int i = 0; i < this->meshList->size(); i++){
//         (*meshList)[i]->renderMesh();
//     }
// }

// void GameObject::render(GLuint uniformModel, GLfloat deltaTime, Shader* outlineShader){
//     update();
//     glStencilFunc(GL_ALWAYS, 1, 0xFF); 
//     glStencilMask(0xFF); 
//     draw(1.0, uniformModel);
    
//     // glBindVertexArray(0);
//     // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//     // glUseProgram(0);


//     // outlineShader->useShader();
//     // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//     // glStencilMask(0x00); 
//     // // glDisable(GL_DEPTH_TEST);
//     // GLfloat uniformModel1 = outlineShader->getModelLocation();

//     //     GLfloat uniformPerspective1 = outlineShader->getPerspectiveLocation();
        
//     //     GLfloat uniformView1 = outlineShader->getViewLocation();

//     //     glm::mat4 perspective = glm::mat4(1);
        
//     //     perspective = glm::perspective(45.0f, (GLfloat)800 / 600, 0.1f, 100.0f);
     
//     //     glUniformMatrix4fv(uniformPerspective1, 1, GL_FALSE, glm::value_ptr(perspective));

//     //     glm::mat4 view = this->scene->camera->calculateViewMatrix();

//     //     glUniformMatrix4fv(uniformView1, 1, GL_FALSE, glm::value_ptr(view));


//     // draw(0.98, uniformModel1);
//     // glStencilMask(0xFF);
//     // glStencilFunc(GL_ALWAYS, 1, 0xFF);   
//     // glEnable(GL_DEPTH_TEST);  
    
// }



void GameObject::interactWithObject(GameObject* go, int type, GLfloat deltaTime){
    go->onInteracted(this, type, deltaTime);
}

GameObject::~GameObject(){
}