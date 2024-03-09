#pragma once
#include <GL/glew.h>
#include <iostream>



#include "stb_image.h"

#include "stb_image_resize.h"

using namespace std;

class Texture{
private:
    GLuint textureId;
    GLuint textureIdSecond;
    int width, height, bitDepth;
    GLuint textureSlot;

    /* data */
public:
    char* fileLocation;
    Texture(char* fileLocation);
    Texture(string filelocation);


    void loadTexture(GLuint textureSlot = 0);
    void useTexture();
    void clearTexture();

    ~Texture();
};


