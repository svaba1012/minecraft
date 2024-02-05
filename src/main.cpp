#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.hpp"

// const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader, uniformModel;

// Vertex shader

static const char* vertexShader = "#version 330                         \n\
                                    \n\
layout (location = 0) in vec3 pos;                        \n\
uniform mat4 model;                  \n\
void main(){                          \n\
        gl_Position = model *  vec4( 0.5 * pos.x , 0.5 * pos.y, 0.5 * pos.z, 1.0);  \n\
}";

static const char* fragmentShader = " \n\
                                    \n\
#version 330                        \n\
                                    \n\
out vec4 color;                     \n\
                                    \n\
void main(){                         \n\
    color =  vec4(0.0, 1.0, 0.0, 1.0);    \n\
}";

void createTriangle()
{

    GLfloat vertices[] = {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        0.0, 1.0, 0.0 };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void addShader(GLuint program, const char* code, GLenum shaderType)
{

    GLuint shaderProgram = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = code;

    GLint codeLength[1];
    codeLength[0] = strlen(code);

    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glShaderSource(shaderProgram, 1, theCode, codeLength);
    glCompileShader(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shaderProgram, sizeof(elog), NULL, elog);
        printf("Error compiling the %s shader: %s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", elog);
        return;
    }

    glAttachShader(program, shaderProgram);

    return;
}

void compileShaders()
{
    shader = glCreateProgram();
    if (!shader)
    {
        printf("Can't create shader program\n");
        return;
    }
    addShader(shader, vertexShader, GL_VERTEX_SHADER);
    addShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Shader linking error: %s\n", elog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Shader validation error: %s\n", elog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
    // if (!glfwInit())
    // {
    //     printf("GLFW initialization failed\n");
    //     glfwTerminate();
    //     return 1;
    // }

    // // glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    // // glfwWindowHint(GLFW_VERSION_MINOR, 3);
    // // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "App", NULL, NULL);

    // if (!window)
    // {
    //     printf("Window setting failed\n");
    //     glfwTerminate();
    //     return 1;
    // }

    // int bufferWidth, bufferHeight;
    // glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // glfwMakeContextCurrent(window);

    // glewExperimental = GL_TRUE;

    // if (glewInit() != GLEW_OK)
    // {
    //     printf("GLEW init failed\n");
    //     glfwDestroyWindow(window);
    //     glfwTerminate();
    //     return 1;
    // }

    // glViewport(0, 0, bufferWidth, bufferHeight);

    // createTriangle();
    // compileShaders();

    // while (!glfwWindowShouldClose(window))
    // {
    //     glfwPollEvents();
    //     glClearColor(0.0, 0.0, 0.0, 1.0);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     glUseProgram(shader);

    //     glm::mat4 model = glm::mat4(1);

    //     printf("%f\n", model[0][0]);

    //     model = glm::translate(model, glm::vec3(0.2, 0.2, 0.2));


        
        
    //     glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    //     glBindVertexArray(VAO);

    //     glDrawArrays(GL_TRIANGLES, 0, 3);

    //     glBindVertexArray(0);

    //     glUseProgram(0);

    //     glfwSwapBuffers(window);
    // }

    // printf("Smekeru\n");

    Window mainWindow;

    if(!mainWindow.init()){
        return 1;
    }
    mainWindow.run();

    return 0;
}