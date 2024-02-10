#include "graphics/Texture.hpp"


#define MIN_TEXTURE_WIDTH 256
#define MIN_TEXTURE_HEIGHT 256

Texture::Texture(char* fileLocation){
    this->textureId = 0;
    this->bitDepth = 0;
    this->width = 0; 
    this->height = 0;
    this->fileLocation = fileLocation;
}

Texture::Texture(string fileLocationStr){
    this->textureId = 0;
    this->bitDepth = 0;
    this->width = 0; 
    this->height = 0;
    const char* fileLocation = fileLocationStr.c_str();
    this->fileLocation = (char* )fileLocation;
}

void Texture::loadTexture(){
    unsigned char* textureData = stbi_load(this->fileLocation, &width, &height, &bitDepth, 0);

    cout<< bitDepth << endl;

    // ! resize image but make them blury
    // assume is divisible by 4
    // if(width < MIN_TEXTURE_WIDTH || height < MIN_TEXTURE_HEIGHT){
    //     int widthScale = MIN_TEXTURE_WIDTH / width;
    //     int heightScale = MIN_TEXTURE_HEIGHT / height;

    //     int scale = widthScale;
    //     if(scale < heightScale){
    //         scale = heightScale;
    //     }

    //     cout << scale << endl;

    //     unsigned char* resizedImage = new unsigned char[width * height * scale * scale * bitDepth];

    //     stbir_resize_uint8(textureData, width, height, 0, resizedImage, width * scale, height * scale, 0, bitDepth);

    //     textureData = resizedImage;
    //     width *= scale;
    //     height *= scale;
    // }
    

    

    if(!textureData){
        printf("Failed to find texture file %s\n", fileLocation);
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    if(bitDepth == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    }else if(bitDepth == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    }
    

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
}




void Texture::useTexture(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::clearTexture(){
    glDeleteTextures(1, &this->textureId);
    this->textureId = 0;
    this->bitDepth = 0;
    this->width = 0; 
    this->height = 0;
    this->fileLocation = "";    
}


Texture::~Texture(){
    clearTexture();
}