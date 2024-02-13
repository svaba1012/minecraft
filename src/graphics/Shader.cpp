#include "graphics/Shader.hpp"

Shader::Shader(/* args */){
    program = glCreateProgram();
    if (!program){
        printf("Can't create shader program\n");
        return;
    }
}

Shader::~Shader(){
    for(int i = 0; i < this->shaderPrograms.size(); i++){
        glDetachShader(this->program, this->shaderPrograms[i]);
    }
    for(int i = 0; i < this->shaderPrograms.size(); i++){
        glDeleteShader(this->shaderPrograms[i]);
    }
    glDeleteProgram(this->program);
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

    shaderPrograms.push_back(shaderProgram);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glShaderSource(shaderProgram, 1, theCode, codeLength);
    glCompileShader(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
    if (!result){
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


GLuint Shader::getUniformLocation(char* uniformName){
    return glGetUniformLocation(this->program, uniformName);
}

GLuint Shader::getAttribLocation(char* attrName){
    return glGetAttribLocation(this->program, attrName);
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
    

