#pragma once
#include <GL/glew.h>
#include <iostream>



#include "stb_image.h"

#include "stb_image_resize.h"

using namespace std;

class Texture{
private:
    GLuint textureId;
    int width, height, bitDepth;

    /* data */
public:
    char* fileLocation;
    Texture(char* fileLocation);
    Texture(string filelocation);

    void loadTexture();
    void useTexture();
    void clearTexture();

    ~Texture();
};


