#include "Camera.hpp"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed){

    this->position = startPosition;
    this->worldUp = startUp;
    this->yaw = startYaw;
    this->pitch = startPitch;
    this->turnSpeed = startTurnSpeed;
    this->movementSpeed = startMoveSpeed;

    this->update();
}

void Camera::set(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch){
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->update();
}

void Camera::set(glm::vec3 position, glm::vec3 front, glm::vec3 right){
    this->front = front;
    this->right = right;
    this->up = glm::normalize(glm::cross(this->right, this->front));
}




void Camera::update(){
    // printf("Camera yaw:%f\n",  yaw);
    this->front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    this->front.y = sin(glm::radians(pitch));
    this->front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->front = glm::normalize(this->front);

    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::drawCursor(){
    int size = 20;

    glm::vec3 cursorTop = this->position + this->front + this->up;
    glm::vec3 cursorBottom = this->position + this->front - this->up;
    glm::vec3 cursorLeft = this->position + this->front - this->right;
    glm::vec3 cursorRight = this->position + this->front + this->right;


    glViewport(0, 0, 800, 600); 
    
    //we want to modify the projection matrix (without this, mesh normals will break)
    glMatrixMode(GL_PROJECTION);

    //clear any previous transforms the projection matrix may contain (otherwise it would be combined with the following glOrtho matrix)
    glLoadIdentity();

    //set the projection (could use glTranslate/glScale but this utility function is simpler)
    glOrtho(0, 800, 0, 600, -1, 1); //left,right,bottom,top,front,back

    //common practice to leave modelview as the current matrix for editing
    glMatrixMode(GL_MODELVIEW);

    
    glLineWidth(2.0);
    glColor3f(1.0,1.0,1.0); // red x

    glBegin(GL_LINES);
        // x aix
        glVertex2f(400 - size / 2, 300);
        glVertex2f(400 + size / 2, 300);

        glVertex2f(400, 300 + size / 2);
        glVertex2f(400, 300 - size / 2);


        // glVertex3f(-cursorLeft[0], -cursorLeft[1], cursorLeft[2]);
        // glVertex3f(-cursorRight[0], -cursorRight[1], cursorRight[2]);

    glEnd();

}

void Camera::keyControl(bool* keys, GLfloat deltaTime){
    if(keys[GLFW_KEY_W]){
        this->position += this->movementSpeed * deltaTime * this->front;
    }
    if(keys[GLFW_KEY_S]){
        this->position -= this->movementSpeed * deltaTime * this->front;
    }
    if(keys[GLFW_KEY_D]){
        this->position += this->movementSpeed * deltaTime * this->right;
    }
    if(keys[GLFW_KEY_A]){
        this->position -= this->movementSpeed * deltaTime * this->right;
    }
}



void Camera::mouseControl(GLfloat xChange, GLfloat yChange){

    this->yaw += xChange * this->turnSpeed;
    this->pitch -= yChange * this->turnSpeed;

    if(this->pitch > 89.0){
        this->pitch = 89.0;
    }else if( this->pitch < -89.0){
        this->pitch = -89.0;
    }

    this->update();
}

glm::mat4 Camera::calculateViewMatrix(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up){
    return glm::lookAt(position, lookAt, up);
}

glm::mat4 Camera::calculateViewMatrix(){
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

GameObject* Camera::getObjectAimingAt(vector<GameObject*> objectList){
    vector<GameObject*> potentialObjects;
    GameObject* objectAimingAt = (GameObject*) NULL;
    GLfloat lowestDistance = 20.0;
    for(int i = 1; i < objectList.size(); i++){
        GameObject* curGo = objectList[i];
        glm::vec3 distanceVec = curGo->pos - this->position;
        GLfloat distance = glm::length(distanceVec);
        GLfloat sinAngle = glm::length(glm::cross(this->front, distanceVec));
        GLfloat cosAngle = glm::dot(this->front, glm::normalize(distanceVec));
        // printf("Spec value: %f\n", sinAngle);
        // printf("Spec cos value: %f\n", cosAngle);
        if(cosAngle < 0.0){
            continue;
        }
        if(sinAngle > curGo->circleOutscribedR){
            continue;
        }
        if(distance > 10){
            continue;
        }
        // if false additional check, if true 100% aiming at
        bool isAimingAt = sinAngle < curGo->circleInscribedR;
        if(!isAimingAt){
            // potentialy aiming at
            // check if it is aiming
            if(!isAimingAt){
                continue;
            }
        }
        // aiming at true;
        if(distance < lowestDistance){
            objectAimingAt = curGo;
            lowestDistance = distance;
        }
        
    }
    return objectAimingAt;
}