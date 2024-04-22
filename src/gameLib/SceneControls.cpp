#include "gameLib/SceneControls.hpp" 
#include <algorithm>

SceneControls::SceneControls(){
    Shader* shader = new Shader();
    this->shader = shader;
    
    char* vertexShaderFilename = "./assets/shaders/gameUiVertexShader.glsl"; 
    char* fragmentShaderFilename = "./assets/shaders/gameUiFragmentShader.glsl"; 
    shader->addShader(vertexShaderFilename, GL_VERTEX_SHADER);
    shader->addShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    this->shader->compileShaders();


    //this->camera = new Camera(glm::vec3(2.5, 2.8, 2.5), glm::vec3(0.0, 1.0, 0.0), 0.0, 0.0, 5.0, 1.0);
}

void SceneControls::init(bool* keys, bool* mouseButtons){
    
    this->time = 0.0;
    this->keys = keys;
    this->mouseButtons = mouseButtons;

    GameUI2D* inventory = new InventoryUI(Character::instance->inventory, 9);
    GameUI2D* quickInventory = new QuickInventoryUI(Character::instance->inventory, 9);
    GameUI2D* charaterStatus = new CharacterStatusUI();

    inventory->init(); 
    quickInventory->init();
    charaterStatus->init();

    
    MinecraftInventoryItem* testItem = new MinecraftInventoryItem(ItemType::getTypeById("diamond_sword"));
     MinecraftInventoryItem* testItem1 = new MinecraftInventoryItem(ItemType::getTypeById("diamond"));

     testItem1->setCount(40);

     MinecraftInventoryItem* testItem2 = new MinecraftInventoryItem(ItemType::getTypeById("diamond"));

     testItem2->setCount(50);
    MinecraftInventoryItem* testItem3 = new MinecraftInventoryItem(ItemType::getTypeById("diamond_helmet"));

    MinecraftInventoryItem* testItem4 = new MinecraftInventoryItem(ItemType::getTypeById("iron_helmet"));

    MinecraftInventoryItem* testItem5 = new MinecraftInventoryItem(ItemType::getTypeById("stick"));

    testItem5->setCount(30);

    MinecraftInventoryItem* testItem6 = new MinecraftInventoryItem(ItemType::getTypeById("gold_ingot"));
    testItem6->setCount(10);

    MinecraftInventoryItem* testItem7 = new MinecraftInventoryItem(ItemType::getTypeById("apple"));

    MinecraftInventoryItem* testItem8 = new MinecraftInventoryItem(ItemType::getTypeById("furnace"));

    MinecraftInventoryItem* testItem9 = new MinecraftInventoryItem(ItemType::getTypeById("crafting_table"));

    MinecraftInventoryItem* testItem10 = new MinecraftInventoryItem(ItemType::getTypeById("gold_ore"));
    MinecraftInventoryItem* testItem11 = new MinecraftInventoryItem(ItemType::getTypeById("iron_ore"));
    MinecraftInventoryItem* testItem12 = new MinecraftInventoryItem(ItemType::getTypeById("charcoal"));

     MinecraftInventoryItem* testItem13 = new MinecraftInventoryItem(ItemType::getTypeById("potato"));


    testItem10->setCount(16);
    testItem11->setCount(32);
    testItem12->setCount(32);
    testItem13->setCount(16);


    ((InventoryUI*)inventory)->setInventoryItem(testItem, 5);
    ((InventoryUI*)inventory)->setInventoryItem(testItem1, 7);
    ((InventoryUI*)inventory)->setInventoryItem(testItem2, 17);
    ((InventoryUI*)inventory)->setInventoryItem(testItem3, 20);
    ((InventoryUI*)inventory)->setInventoryItem(testItem4, 30);
    ((InventoryUI*)inventory)->setInventoryItem(testItem5, 31);
    ((InventoryUI*)inventory)->setInventoryItem(testItem6, 32);
    ((InventoryUI*)inventory)->setInventoryItem(testItem7, 33);
    ((InventoryUI*)inventory)->setInventoryItem(testItem8, 21);
    ((InventoryUI*)inventory)->setInventoryItem(testItem9, 22);
    ((InventoryUI*)inventory)->setInventoryItem(testItem10, 10);
    ((InventoryUI*)inventory)->setInventoryItem(testItem11, 11);
    ((InventoryUI*)inventory)->setInventoryItem(testItem12, 12);
    ((InventoryUI*)inventory)->setInventoryItem(testItem13, 13);



    activeUI = inventory;
    this->gameUIs.push_back(inventory);
    this->gameUIs.push_back(quickInventory);
    this->gameUIs.push_back(charaterStatus);
    
    this->shader->useShader();
}

bool* SceneControls::getKeys(){
    return this->keys;
}

GLfloat SceneControls::getXChange(){
    GLfloat change = this->xChange;
    this->xChange = 0.0;
    return change;
}

GLfloat SceneControls::getYChange(){
    GLfloat change = this->yChange;
    this->yChange = 0.0;
    return change;
}

void SceneControls::addNewGameUIObject(GameUI2D* gUIO){
    this->gameUIs.push_back(gUIO);
}

void SceneControls::render(){
        //calc seconds per frame - deltaTime
        nowTime = glfwGetTime();
        deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        this->time += deltaTime;


        activeUI->pump_input(Window::window);

        
            
            for(int i = 0; i < gameUIs.size(); i++){
                // if(isInventoryOpen || i > 0){
                    gameUIs[i]->render();
                // }
            }
        
        
        
        if(Character::instance->isInventoryUIOpen){
            glfwSetInputMode(Window::window, GLFW_CURSOR , GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode(Window::window, GLFW_CURSOR , GLFW_CURSOR_DISABLED);
        }

        for(int i = 0; i < gameUIs.size(); i++){
            // if(isInventoryOpen || i > 0){
                gameUIs[i]->device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
            // }
        }
        
            

    
        // printf("FPS: %f\n", 1 / deltaTime);
        //glfwSwapBuffers(this->glfwWindow);
}

void SceneControls::setCamera(){

}
glm::mat4 SceneControls::getViewMatrix(){
    return glm::mat4(1);
}

void SceneControls::onKeys(int key, int code, int action, int mode){
    // if(action == GLFW_PRESS){
    //     if(key == GLFW_KEY_E){
    //         isInventoryOpen = !isInventoryOpen;
    //     }
    // }

}


SceneControls::~SceneControls()
{
}
