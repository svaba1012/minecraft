#include "Window.hpp"

#include "game/BlockType.hpp"
#include "minecraft/ItemType.hpp"

#define NK_IMPLEMENTATION
#include "gameLib/extern/use_nuklear.h"



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

struct nk_image icon_load(const char *filename){
    int x,y,n;
    GLuint tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data) printf("[SDL]: failed to load image: %s", filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int)tex);
}

void inventory_box( struct nk_context *ctx, struct nk_image image){
    // struct nk_context *ctx = &context;
    struct nk_style_window old_style = ctx->style.window;
    ctx->style.window.group_padding = nk_vec2(0, 0);
    if (nk_group_begin(ctx, "Group", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        // nk_layout_row_dynamic(ctx, 50, 1);
        nk_layout_row_static(ctx, 50, 50, 1);
        nk_image(ctx, image);
        nk_group_end(ctx);
    }
    ctx->style.window = old_style;
}

void drawInventory(struct nk_context *ctx){
    const int INVENTORY_ITEM_SIZE = 50;
    const int xPos = 130, yPos = 10;
    if (nk_begin(ctx, "Inventory", nk_rect(xPos, yPos, 9.9 * INVENTORY_ITEM_SIZE, 9.6 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
        struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
        float rowWidths[6] = {1, 3, 1, 2.2, 1, 1};
        for(int i = 0; i < 6; i++){
            rowWidths[i] *= INVENTORY_ITEM_SIZE;
        }

        nk_layout_row(ctx, NK_STATIC, 4.7 * INVENTORY_ITEM_SIZE, 6, rowWidths);
        struct nk_style_window old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            // nk_layout_row_dynamic(ctx, 50, 1);
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image);
            inventory_box(ctx, image);
            inventory_box(ctx, image);
            inventory_box(ctx, image);
            nk_group_end(ctx);
        }
        ctx->style.window = old_style;
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }

        old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            inventory_box(ctx, image);
            nk_group_end(ctx);
        }
        ctx->style.window = old_style;
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.3, 1);
            nk_label(ctx, "Crafting", NK_TEXT_LEFT);
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 2);
            inventory_box(ctx, image);
            inventory_box(ctx, image);
            inventory_box(ctx, image);
            inventory_box(ctx, image);

            // ! recepies button
            nk_group_end(ctx);
        }

        old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image);
            
            nk_group_end(ctx);
        }
        ctx->style.window = old_style;
        old_style = ctx->style.window;

        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image);
            
            nk_group_end(ctx);
        }
        ctx->style.window = old_style;

        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 9; j++){
                inventory_box(ctx, image);
            }
        }
        nk_layout_row_dynamic(ctx, 0.2 * INVENTORY_ITEM_SIZE, 1);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
        for(int j = 0; j < 9; j++){
            inventory_box(ctx, image);
        }

        // inventory_box(ctx, image);
    }
    nk_end(ctx);
}

void drawQuickInventory(struct nk_context *ctx){
    const int INVENTORY_ITEM_SIZE = 50;
    const int xPos = 130, yPos = 520;
    if (nk_begin(ctx, "QuickInventory", nk_rect(xPos, yPos, 9.9 * INVENTORY_ITEM_SIZE, 1.25 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
        struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
        for(int j = 0; j < 9; j++){
            inventory_box(ctx, image);
        }
        nk_end(ctx);
    }
}


void Window::run(){

    GameUI2D ui2dQuickInventory;
    GameUI2D ui2dInventory;
    
    ui2dQuickInventory.uiRender = drawQuickInventory;
    ui2dQuickInventory.init();

    ui2dInventory.uiRender = drawInventory;
    ui2dInventory.init();

    this->curScene->init(this->keys, this->mouseButtons);
    while (!glfwWindowShouldClose(this->glfwWindow)){

        ui2dQuickInventory.pump_input(this->glfwWindow);

        ui2dInventory.pump_input(this->glfwWindow);

        ui2dQuickInventory.draw();
        if(isInventoryOpen){
            ui2dInventory.draw();
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

        
        ui2dQuickInventory.device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
        if(isInventoryOpen){
            ui2dInventory.device_draw(WIDTH, HEIGHT, NK_ANTI_ALIASING_ON);
        }

        glfwSwapBuffers(this->glfwWindow);
    }

}


Window::~Window(){
    
}