#include "Window.hpp"

#include "game/BlockType.hpp"
#include "minecraft/ItemType.hpp"
#include "gameLib/inventory/Inventory.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"

#define NK_IMPLEMENTATION
#include "gameLib/extern/use_nuklear.h"

#define MIN_TEXTURE_WIDTH 50
#define MIN_TEXTURE_HEIGHT 50

GLFWwindow* Window::window;


Window::Window(){
    for(int i = 0; i < 1024; i++){
        this->keys[i] = 0;
    }
    this->mouseFirstMoved = true;
    
    BlockType::init();
    ItemType::init();

    BlockType::getTypeByName("Ancient Debris");
    
}

bool Window::init(){
    if (!glfwInit())
    {
        printf("GLFW initialization failed\n");
        glfwTerminate();
        return false;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "App", NULL, NULL);

    if (!this->glfwWindow)
    {
        printf("Window setting failed\n");
        glfwTerminate();
        return false;
    }

    Window::window = this->glfwWindow;

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(this->glfwWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(this->glfwWindow);

    this->createCallbacks();

    glfwSetInputMode(this->glfwWindow, GLFW_CURSOR ,GLFW_CURSOR_DISABLED);

    glfwSetInputMode(this->glfwWindow, GLFW_CURSOR , GLFW_CURSOR_NORMAL);



    // Create the cursor object
    
    

    // glfwWidnowHint(GLFW_CENTER_CURSOR)
    // GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

    // glfwSetCursor(this->glfwWindow, cursor);
    
    

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW init failed\n");
        glfwDestroyWindow(this->glfwWindow);
        glfwTerminate();
        return false;
    }

    GameUI2D::device_init(this->glfwWindow);
    

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(this->glfwWindow, this);


    

    this->curScene = new Scene();

    for(int i = 0; i < 3; i++){
        this->mouseButtons[i] = false;
    }
    
    return true;
}

void Window::createCallbacks(){
    glfwSetKeyCallback(this->glfwWindow, handleKeys);
    glfwSetMouseButtonCallback(this->glfwWindow, handleMouseButtons);
    glfwSetCursorPosCallback(this->glfwWindow, handleMouse);
}


void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode){

    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if(key == GLFW_KEY_E){
            theWindow->isInventoryOpen = !theWindow->isInventoryOpen;
        }
        if(key >= 0 && key < 1024){
            theWindow->keys[key] = true;
        }
    }else if(action == GLFW_RELEASE){
        if(key >= 0 && key < 1024){
            theWindow->keys[key] = false;
        }
    }

}
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos){
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(theWindow->mouseFirstMoved){
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = yPos - theWindow->lastY;

    theWindow->curScene->xChange = theWindow->xChange;
    theWindow->curScene->yChange = theWindow->yChange;


    // theWindow->camera->mouseControl(theWindow->xChange, theWindow->yChange);
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void Window::handleMouseButtons(GLFWwindow* window, int button, int action, int mods){
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(action == GLFW_PRESS){
        // currently supporting left, midle and right button
        if(button < 3){
            theWindow->mouseButtons[button] = true;
        }
    }else if(action == GLFW_RELEASE){
        if(button < 3){
            theWindow->mouseButtons[button] = false;
        }
    }
    
}


bool* Window::getKeys(){
    return this->keys;
}

GLfloat Window::getXChange(){
    GLfloat change = this->xChange;
    this->xChange = 0.0;
    return change;
}

GLfloat Window::getYChange(){
    GLfloat change = this->yChange;
    this->yChange = 0.0;
    return change;
}



void Window::run(){

    // GameUI2D ui2dQuickInventory;
    // GameUI2D ui2dInventory;
    
    // ui2dQuickInventory.uiRender = drawQuickInventory;
    // ui2dQuickInventory.init();

    // ui2dInventory.uiRender = drawInventory;
    // ui2dInventory.init();

    GameUI2D* inventory = new Inventory(27, 9);

    inventory->init(); 

    this->curScene->init(this->keys, this->mouseButtons);

    
    MinecraftInventoryItem* testItem = new MinecraftInventoryItem(ItemType::getTypeById("diamond_sword"));
     MinecraftInventoryItem* testItem1 = new MinecraftInventoryItem(ItemType::getTypeById("diamond"));

     testItem1->setCount(40);

     MinecraftInventoryItem* testItem2 = new MinecraftInventoryItem(ItemType::getTypeById("diamond"));

     testItem2->setCount(50);
    ((Inventory*)inventory)->setInventoryItem(testItem, 5);
    ((Inventory*)inventory)->setInventoryItem(testItem1, 7);
    ((Inventory*)inventory)->setInventoryItem(testItem2, 17);

    while (!glfwWindowShouldClose(this->glfwWindow)){

        // ui2dQuickInventory.pump_input(this->glfwWindow);

        // ui2dInventory.pump_input(this->glfwWindow);

        inventory->pump_input(this->glfwWindow);

        // ui2dQuickInventory.draw();
        if(isInventoryOpen){
            inventory->render();
            // ui2dInventory.draw();
        }

        glfwPollEvents();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glEnable(GL_DEPTH_TEST);
  
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnable(GL_DEPTH_TEST);
        this->curScene->render();

        
        // ui2dQuickInventory.device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
        if(isInventoryOpen){
            glfwSetInputMode(this->glfwWindow, GLFW_CURSOR , GLFW_CURSOR_NORMAL);

            inventory->device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
            // ui2dInventory.device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
        }else{
            glfwSetInputMode(this->glfwWindow, GLFW_CURSOR , GLFW_CURSOR_DISABLED);

        }

        glfwSwapBuffers(this->glfwWindow);
    }

}


Window::~Window(){
    
}