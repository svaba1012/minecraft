#include "Window.hpp"

#include "game/BlockType.hpp"




Window::Window(){
    for(int i = 0; i < 1024; i++){
        this->keys[i] = 0;
    }
    this->mouseFirstMoved = true;
    
    BlockType::init();
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

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(this->glfwWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(this->glfwWindow);

    this->createCallbacks();

    glfwSetInputMode(this->glfwWindow, GLFW_CURSOR ,GLFW_CURSOR_DISABLED);

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
    
    this->curScene->init(this->keys, this->mouseButtons);
    while (!glfwWindowShouldClose(this->glfwWindow)){

        glfwPollEvents();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->curScene->render();
        

        glfwSwapBuffers(this->glfwWindow);
    }

}


Window::~Window(){
    
}