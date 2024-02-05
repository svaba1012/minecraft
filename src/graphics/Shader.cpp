#include "graphics/Shader.hpp"

Shader::Shader(/* args */){
    program = glCreateProgram();
    if (!program){
        printf("Can't create shader program\n");
        return;
    }
}

Shader::~Shader(){
}

void Shader::createTriangle(GLfloat* vertices, GLuint verticesLen){

    GLfloat vertices1[] = {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        0.0, 1.0, 0.0 };

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesLen * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


bool Shader::addShader(char* filename, GLenum shaderType){
    if (!this->program){
        printf("Can't create shader program\n");
        return false;
    }
    FILE* shaderFile = fopen(filename, "r");
    if(!shaderFile){
        printf("Can't open shader file\n");
        return false;
    }
    fseek(shaderFile, 0, SEEK_END);
    int fileSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);
    printf("%d\n", fileSize);
    char* shaderCode = (char*)malloc(fileSize + 1);
    if(fread(shaderCode, 1, fileSize, shaderFile) != fileSize){
        printf("Error reading shader file\n");
        return false;
    }
    shaderCode[fileSize] = 0;
    

    GLuint shaderProgram = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glShaderSource(shaderProgram, 1, theCode, codeLength);
    glCompileShader(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shaderProgram, sizeof(elog), NULL, elog);
        printf("Error compiling the %s shader: %s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", elog);
        return false;
    }
    glAttachShader(this->program, shaderProgram);

    return true;
}

void Shader::compileShaders(){


    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(this->program, sizeof(elog), NULL, elog);
        printf("Shader linking error: %s\n", elog);
        return;
    }

    glValidateProgram(this->program);
    glGetProgramiv(this->program, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(this->program, sizeof(elog), NULL, elog);
        printf("Shader validation error: %s\n", elog);
        return;
    }

}

GLuint Shader::getModelLocation(){
    return glGetUniformLocation(this->program, "model");
}
GLuint Shader::getPerspectiveLocation(){
    return glGetUniformLocation(this->program, "perspective");
}
GLuint Shader::getViewLocation(){
    return glGetUniformLocation(this->program, "view");
}

GLuint Shader::getAmbientIntensityLocation(){
    return glGetUniformLocation(this->program, "directionalLight.ambientIntensity");
}

GLuint Shader::getAmbientColorLocation(){
    return glGetUniformLocation(this->program, "directionalLight.color");
}



void Shader::useShader(){
    
    glUseProgram(this->program);
}
    

