#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.hpp"
#include "./Camera.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "./Light.hpp"
#include "game/Block.hpp"
#include "gameLib/Scene.hpp"
#include "game/OpenWorldScene.hpp"
#include "gameLib/SceneControls.hpp"
#include "gameLib/ui/GameUI2D.hpp"

using namespace std;

class Window{
private:
    /* data */
    GLFWwindow* glfwWindow;
    const GLint WIDTH = 800, HEIGHT = 600;
    void createCallbacks();
    
    Scene* curScene;
    Scene* overlayScene;

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);

    bool keys[1024];
    bool mouseButtons[3];
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouseButtons(GLFWwindow* window, int button, int action, int mods);

    
public:
    // ! remove to Character
    static bool isInventoryOpen;
    static GLFWwindow* window;
    Window(/* args */);

    bool init();
    void run();

    bool* getKeys();
    GLfloat getXChange();
    GLfloat getYChange();
    
    ~Window();

};



