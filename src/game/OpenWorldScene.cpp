#include "game/OpenWorldScene.hpp" 
#include <algorithm>

OpenWorldScene::OpenWorldScene(){
    Shader* shader = new Shader();
    this->shader = shader;
    
    char* vertexShaderFilename = "./assets/shaders/vertexShader.glsl"; 
    char* fragmentShaderFilename = "./assets/shaders/fragmentShader.glsl"; 
    shader->addShader(vertexShaderFilename, GL_VERTEX_SHADER);
    shader->addShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    this->shader->compileShaders();

    

    this->camera = new Camera(glm::vec3(2.5, 2.8, 2.5), glm::vec3(0.0, 1.0, 0.0), 0.0, 0.0, 5.0, 1.0);
    
}

void OpenWorldScene::init(bool* keys, bool* mouseButtons){
    Recipe::loadRecipes();
    this->time = 0.0;
    this->keys = keys;
    this->mouseButtons = mouseButtons;
    Character* character = new Character(this);
    character->generateMeshes();
    character->pos = glm::vec3(10.0, 10.0 , 10.0);
    this->objectAttachedCamera = character;

    userControllableGameObjects.push_back(character);
    gameObjects.push_back(character);
    
    // Simple world
    
    


    for(int i = -15; i < 15; i++){
        for(int j = -15; j < 15; j++){
            Block* block = new Block(this, glm::vec3((GLfloat)i, 0.0, (GLfloat)j), glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
            block->setType("Dirt", "Dirt");
            block->generateMeshes();
            gameObjects.push_back(block);
        }   
    }

    // for(int i = 0; i < 10; i++){
    //     Item* item = new Item(this, glm::vec3((GLfloat)i, 1.01, (GLfloat)i), glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
    //     item->setTypeById("coal_ore");
    //     item->generateMeshes();
    //     gameObjects.push_back(item);
    // }

    printf("Napravljen pod\n");

    for(int i = 1; i < 7; i++){
        Block* block = new Block(this, glm::vec3((GLfloat)0.0, (GLfloat)i, (GLfloat)0.0), glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
        block->setType("Logs", "Oak Log");
        block->generateMeshes();
        gameObjects.push_back(block);
    }

    for(int k = 4; k < 7; k++){
        for(int j = -2; j < 3; j++){
            for(int i = -2; i < 3; i++){
                Block* block = new Block(this, glm::vec3((GLfloat)i, (GLfloat)k, (GLfloat)j), glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
                block->setType("Leaves", "Spruce Leaves");
                block->generateMeshes();
                gameObjects.push_back(block);
            }
        }
    }
    
    


    Block* block = new Block(this, glm::vec3(20.0, 0.0, 20.0), glm::vec3(0.0, 0.0, 0.0) ,glm::vec3(1.0, 1.0, 1.0));
    block->setType("Logs", "Oak Log");
            block->generateMeshes();
            gameObjects.push_back(block);
    
    

    light = Light(1.0, 1.0, 1.0, 0.4);

    this->shader->useShader();

    // this->shader->setTextureUniform("texture1", 0);
    // this->shader->setTextureUniform("texture2", 1);
}

bool* OpenWorldScene::getKeys(){
    return this->keys;
}

// bool Scene::collisionCheck(GameObject* go){
//     return false;
// }


GLfloat OpenWorldScene::getXChange(){
    GLfloat change = this->xChange;
    this->xChange = 0.0;
    return change;
}

GLfloat OpenWorldScene::getYChange(){
    GLfloat change = this->yChange;
    this->yChange = 0.0;
    return change;
}

void OpenWorldScene::addNewGameObject(GameObject* go){
    this->gameObjects.push_back(go);
}

void OpenWorldScene::addNewCollectable(Item* go){
    this->gameObjects.push_back(go);
    this->itemCollectables.push_back(go);
}

void OpenWorldScene::removeCollectable(Item* go){
    vector<GameObject*>::iterator position = std::find(gameObjects.begin(), gameObjects.end(), go);
    if (position != gameObjects.end()) // == myVector.end() means the element was not found
    gameObjects.erase(position);

    vector<Item*>::iterator position1 = std::find(itemCollectables.begin(), itemCollectables.end(), go);
    if (position1 != itemCollectables.end()) // == myVector.end() means the element was not found
    itemCollectables.erase(position1);
    delete go;
}


void OpenWorldScene::setCamera(){

    if(this->objectAttachedCamera){
        this->camera->set(this->objectAttachedCamera->pos -this->objectAttachedCamera->front -this->objectAttachedCamera->front-this->objectAttachedCamera->front-this->objectAttachedCamera->front, glm::vec3(0.0, 1.0, 0.0), this->objectAttachedCamera->yaw, this->objectAttachedCamera->pitch + 20.0);
        

        //  this->camera->set(this->objectAttachedCamera->pos -this->objectAttachedCamera->front -this->objectAttachedCamera->front, this->objectAttachedCamera->front, this->objectAttachedCamera->right);
    }
}

glm::mat4 OpenWorldScene::getViewMatrix(){

    if(this->objectAttachedCamera){
        return this->camera->calculateViewMatrix(this->objectAttachedCamera->pos - this->objectAttachedCamera->front - this->objectAttachedCamera->front- this->objectAttachedCamera->front, this->objectAttachedCamera->pos, this->objectAttachedCamera->up );
    }
}

GameObject* objectAimingAt = (GameObject*) NULL;
GameObject* oldObjectAimingAt = (GameObject*) NULL;

void OpenWorldScene::render(){
        //calc seconds per frame - deltaTime
        nowTime = glfwGetTime();
        deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        this->time += deltaTime;

        // printf("Init    fine\n");

        // listen for user inputs
        
        // ItemType* diamondItem = ItemType::getTypeById("diamond");
        // ItemType* stickItem = ItemType::getTypeById("stick");
        // ItemType* goldItem = ItemType::getTypeById("gold_ingot");
        // ItemType* oakLog = ItemType::getTypeById("oak_planks");

        // ItemType* pattern[3][3] = {{NULL, NULL, NULL}, {NULL, goldItem,  NULL}, {NULL, NULL, goldItem}};
        // Recipe::craftItem(pattern);
        
        for(int i = 0; i < this->userControllableGameObjects.size(); i++){
            this->userControllableGameObjects[i]->keyControl(this->getKeys(), deltaTime);
            this->userControllableGameObjects[i]->mouseControl(this->getXChange(), this->getYChange());
        }

        // potentialy move objects
        // set velocities
        // ...

        // check for collisions
        int isColliding = 0;

        
        
        for(int i = 0; i < this->gameObjects.size(); i++){
            GameObject* curGo = gameObjects[i];
            if(curGo->isStatic){
                continue;
            }

            
                
            for(int j = 0; j < this->gameObjects.size(); j++){
                if((!curGo->isCollideable && !gameObjects[j]->isCollideable) || curGo == gameObjects[j]){
                    continue;
                }
                isColliding = curGo->isCollidingWith(gameObjects[j], deltaTime);
                if(isColliding){
                    break;
                }
            }
            
            

            if(!isColliding){
                curGo->applyMovement(deltaTime);
            }else{
                float xVel = curGo->velocity.x;
                float yVel = curGo->velocity.y;
                float zVel = curGo->velocity.z;
                
                switch(isColliding){
                    case 1: xVel *= -1;
                            //xVel = 0.0;
                            break;
                    case 2: yVel *= -1;
                            //yVel = 0.0;
                            break;
                    case 3: zVel *= -1;
                            //zVel = 0.0;
                            break;         
                }

                if(isColliding == 2 && yVel > 0.0){
                    //curGo->velocity = glm::vec3(0.0, 0.0, 0.0);
                    continue;
                }
                    curGo->velocity = glm::vec3(xVel, yVel, zVel);
                    curGo->applyMovement(deltaTime);
                //curGo->acceleration = glm::vec3(0.0, 0.0, 0.0);
                
            }
        }
        if(!isColliding){
            // printf("Nema kolizije\n");
        }else{
            // printf("Ima kolizije\n");
        }
        
        // apply movements


        this->setCamera();
        this->camera->drawCursor();


        for(int i = 0; i < itemCollectables.size(); i++){
            if(glm::length(itemCollectables[i]->pos - Character::instance->pos) < 1.5){
                printf("Collecting this item...\n");
                Character::instance->collectItem(itemCollectables[i]);
            }
        }


        
        objectAimingAt = this->camera->getObjectAimingAt(this->gameObjects); 
        if(objectAimingAt != oldObjectAimingAt){
            if(oldObjectAimingAt){
                oldObjectAimingAt->toogleOutline();
                userControllableGameObjects[0]->interactWithObject(oldObjectAimingAt, 2, deltaTime);
            }
            oldObjectAimingAt = objectAimingAt;
            if(objectAimingAt){
                objectAimingAt->toogleOutline();
                // printf("Aiming at\n");
            }

        }


        if(!mouseButtons[0] && objectAimingAt && isHitting){
            isHitting = false;
            userControllableGameObjects[0]->interactWithObject(objectAimingAt, 2, deltaTime);
        }

        if(mouseButtons[0] && objectAimingAt){
            isHitting = true;
            userControllableGameObjects[0]->interactWithObject(objectAimingAt, 0, deltaTime);
            if(objectAimingAt->health < 0.0){
                vector<GameObject*>::iterator position = std::find(gameObjects.begin(), gameObjects.end(), objectAimingAt);
                if (position != gameObjects.end()) // == myVector.end() means the element was not found
                gameObjects.erase(position);
                if(oldObjectAimingAt == objectAimingAt){
                    oldObjectAimingAt = (GameObject*)NULL;
                }
                delete objectAimingAt;
                objectAimingAt = (GameObject*)NULL;
                // mouseButtons[0] = false;
            }
            
        }

        
        
        if(mouseButtons[1] && objectAimingAt && isRightRelesed){
            userControllableGameObjects[0]->interactWithObject(objectAimingAt, 1, deltaTime);
        }

        if(mouseButtons[1]){
            isRightRelesed = false;
        }else{
            isRightRelesed = true;
        }
        
        // this->camera->keyControl(this->getKeys(), deltaTime);
        // this->camera->mouseControl(this->getXChange(), this->getYChange());
        this->shader->useShader();

        uniformModel = this->shader->getModelLocation();

        uniformPerspective = this->shader->getPerspectiveLocation();
        
        uniformView = this->shader->getViewLocation();

        uniformLightColor = this->shader->getAmbientColorLocation();

        uniformLightIntensity = this->shader->getAmbientIntensityLocation();

        glm::mat4 perspective = glm::mat4(1);
        
        perspective = glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
     
        glUniformMatrix4fv(uniformPerspective, 1, GL_FALSE, glm::value_ptr(perspective));

        glm::mat4 view = this->camera->calculateViewMatrix();
        // glm::mat4 view = this->getViewMatrix();


        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));


        light.useLight(uniformLightColor, uniformLightIntensity);

        // for(int i = 0; i < userControllableGameObjects.size(); i++){
        //     userControllableGameObjects[i]->render(uniformModel, deltaTime);
        // }

        
        for(int i = 0; i < gameObjects.size(); i++){
            gameObjects[i]->render(uniformModel, deltaTime);
        }


        

        
        
        

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
        // printf("FPS: %f\n", 1 / deltaTime);
        //glfwSwapBuffers(this->glfwWindow);
}

void OpenWorldScene::onKeys(int key, int code, int action, int mode){
    for(int i = 0; i < this->userControllableGameObjects.size(); i++){
        userControllableGameObjects[i]->onKeys(key, code, action, mode);
    }
}

OpenWorldScene::~OpenWorldScene()
{
}
