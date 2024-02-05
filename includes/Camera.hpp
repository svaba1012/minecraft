#pragma once
#include <stdio.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game/GameObject.hpp"

using namespace std;

class GameObject;

class Camera{
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        GLfloat yaw;
        GLfloat pitch;

        GLfloat movementSpeed;
        GLfloat turnSpeed;

        void update();
    public:
        Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

        void set(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);
        void set(glm::vec3 position, glm::vec3 front, glm::vec3 right);

        
        void keyControl(bool* keys, GLfloat deltaTime);
        void mouseControl(GLfloat xChange, GLfloat yChange);

        void drawCursor();


        GameObject* getObjectAimingAt(vector<GameObject*> objectList);

        glm::mat4 calculateViewMatrix();

        glm::mat4 calculateViewMatrix(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);

        ~Camera();


};