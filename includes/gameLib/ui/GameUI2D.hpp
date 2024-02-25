#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// #ifndef NK_PRIVATE
// #define NK_PRIVATE
// #define NK_INCLUDE_FIXED_TYPES
// #define NK_INCLUDE_STANDARD_IO
// #define NK_INCLUDE_DEFAULT_ALLOCATOR
// #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
// #define NK_INCLUDE_FONT_BAKING
// #define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
// #include "gameLib/extern/nuklear.h"
// #endif

#include "gameLib/extern/use_nuklear.h"



// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #endif

#include "graphics/Texture.hpp"
#include "graphics/Shader.hpp"

/* macros */
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#define NK_SHADER_VERSION "#version 150\n"

/* ===============================================================
 *
 *                          DEVICE
 *
 * ===============================================================*/


typedef void (*ui2dFunc) (struct nk_context *ctx);

struct nk_glfw_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

struct device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture tex_null;
    
    GLuint vbo, vao, ebo;
    // ? could be removed
    // GLuint prog;
    // GLuint vert_shdr;
    // GLuint frag_shdr;

    Shader* shader;

    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;

    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};




class GameUI2D{
    private:
        static bool isDevInitialized;
        static struct device device;
        static struct nk_font_atlas atlas;
        static struct nk_context context;
        
        // static Shader* shader;


    public:
    // ! *dev can be removed as arg

        ui2dFunc uiRender;
        static void device_init(GLFWwindow *win);
        void device_upload_atlas(const void *image, int width, int height);
        static void device_shutdown();

        void device_draw(int width, int height,enum nk_anti_aliasing AA);
        void pump_input(GLFWwindow *win);

        void init();
        void draw();
        void free();

};