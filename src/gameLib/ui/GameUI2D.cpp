#include "gameLib/ui/GameUI2D.hpp"

// #define NK_IMPLEMENTATION
#include "gameLib/extern/use_nuklear.h"
// #include "gameLib/extern/nuklear.h"

// #include "gameLib/extern/use_nuklear.h"

bool GameUI2D::isDevInitialized;
struct device GameUI2D::device;
struct nk_font_atlas GameUI2D::atlas;
struct nk_context GameUI2D::context;


static void error_callback(int e, const char *d){printf("Error %d: %s\n", e, d);}
static void text_input(GLFWwindow *win, unsigned int codepoint)
{nk_input_unicode((struct nk_context*)glfwGetWindowUserPointer(win), codepoint);}
static void scroll_input(GLFWwindow *win, double _, double yoff)
{UNUSED(_);nk_input_scroll((struct nk_context*)glfwGetWindowUserPointer(win), nk_vec2(0, (float)yoff));}





void GameUI2D::device_init(GLFWwindow *win){
    struct nk_context *ctx = &context;
    glfwSetWindowUserPointer(win, ctx);
    glfwSetCharCallback(win, text_input);
    glfwSetScrollCallback(win, scroll_input);
    struct device *dev = &GameUI2D::device;
    GLint status;

    // ? could be removed 
    // static const GLchar *vertex_shader =
    //     NK_SHADER_VERSION
    //     "uniform mat4 ProjMtx;\n"
    //     "in vec2 Position;\n"
    //     "in vec2 TexCoord;\n"
    //     "in vec4 Color;\n"
    //     "out vec2 Frag_UV;\n"
    //     "out vec4 Frag_Color;\n"
    //     "void main() {\n"
    //     "   Frag_UV = TexCoord;\n"
    //     "   Frag_Color = Color;\n"
    //     "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
    //     "}\n";
    // static const GLchar *fragment_shader =
    //     NK_SHADER_VERSION
    //     "precision mediump float;\n"
    //     "uniform sampler2D Texture;\n"
    //     "in vec2 Frag_UV;\n"
    //     "in vec4 Frag_Color;\n"
    //     "out vec4 Out_Color;\n"
    //     "void main(){\n"
    //     "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    //     "}\n";

    nk_buffer_init_default(&dev->cmds);

    // ! SHADER
    dev->shader = new Shader();
    char* vertexShaderFilename = "./assets/shaders/gameUiVertexShader.glsl"; 
    char* fragmentShaderFilename = "./assets/shaders/gameUiFragmentShader.glsl"; 
    dev->shader->addShader(vertexShaderFilename, GL_VERTEX_SHADER);
    dev->shader->addShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
    dev->shader->compileShaders();

    // ? could be removed
    // dev->prog = glCreateProgram();
    // dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
    // dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
    // glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
    // glCompileShader(dev->vert_shdr);
    // glCompileShader(dev->frag_shdr);
    // glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
    // assert(status == GL_TRUE);
    // glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
    // assert(status == GL_TRUE);
    // glAttachShader(dev->prog, dev->vert_shdr);
    // glAttachShader(dev->prog, dev->frag_shdr);
    // glLinkProgram(dev->prog);
    // glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
    // assert(status == GL_TRUE);

    // ! SHADER

    dev->uniform_tex = dev->shader->getUniformLocation("Texture"); 
    //glGetUniformLocation(dev->prog, "Texture");
    dev->uniform_proj = dev->shader->getUniformLocation("ProjMtx");
    //glGetUniformLocation(dev->prog, "ProjMtx");
    dev->attrib_pos = dev->shader->getAttribLocation("Position");
    // glGetAttribLocation(dev->prog, "Position");
    dev->attrib_uv = dev->shader->getAttribLocation("TexCoord");
    // glGetAttribLocation(dev->prog, "TexCoord");
    dev->attrib_col = dev->shader->getAttribLocation("Color");
    // glGetAttribLocation(dev->prog, "Color");
    printf("%d\n", dev->attrib_col);

    {
        /* buffer setup */
        GLsizei vs = sizeof(struct nk_glfw_vertex);
        size_t vp = offsetof(struct nk_glfw_vertex, position);
        size_t vt = offsetof(struct nk_glfw_vertex, uv);
        size_t vc = offsetof(struct nk_glfw_vertex, col);

        glGenBuffers(1, &dev->vbo);
        glGenBuffers(1, &dev->ebo);
        glGenVertexArrays(1, &dev->vao);

        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glEnableVertexAttribArray((GLuint)dev->attrib_pos);
        glEnableVertexAttribArray((GLuint)dev->attrib_uv);
        glEnableVertexAttribArray((GLuint)dev->attrib_col);

        glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GameUI2D::device_upload_atlas(const void *image, int width, int height){
    struct device *dev = &GameUI2D::device;
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void GameUI2D::device_shutdown(){
    struct device *dev = &GameUI2D::device;
    delete dev->shader;
    // ? could be removed
    // glDetachShader(dev->prog, dev->vert_shdr);
    // glDetachShader(dev->prog, dev->frag_shdr);
    // glDeleteShader(dev->vert_shdr);
    // glDeleteShader(dev->frag_shdr);
    // glDeleteProgram(dev->prog);
    glDeleteTextures(1, &dev->font_tex);
    glDeleteBuffers(1, &dev->vbo);
    glDeleteBuffers(1, &dev->ebo);
    nk_buffer_free(&dev->cmds);
}

void GameUI2D::device_draw(int width, int height, enum nk_anti_aliasing AA){
    struct device *dev = &GameUI2D::device;
    struct nk_context *ctx = &context;
    GLfloat ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f,-2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f,1.0f, 0.0f, 1.0f},
    };
    ortho[0][0] /= (GLfloat)width;
    ortho[1][1] /= (GLfloat)height;

    /* setup global state */
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    /* setup program */
    // glUseProgram(dev->prog);
    dev->shader->useShader();
    glUniform1i(dev->uniform_tex, 0);
    glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
    {
        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command *cmd;
        void *vertices, *elements;
        const nk_draw_index *offset = NULL;

        /* allocate vertex and element buffer */
        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_MEMORY, NULL, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_MEMORY, NULL, GL_STREAM_DRAW);

        /* load draw vertices & elements directly into vertex + element buffer */
        vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        {
            /* fill convert configuration */
            struct nk_convert_config config;
            static const struct nk_draw_vertex_layout_element vertex_layout[] = {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, position)},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, uv)},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_glfw_vertex, col)},
                {NK_VERTEX_LAYOUT_END}
            };
            // NK_MEMSET(&config, 0, sizeof(config));
            config.vertex_layout = vertex_layout;
            config.vertex_size = sizeof(struct nk_glfw_vertex);
            config.vertex_alignment = NK_ALIGNOF(struct nk_glfw_vertex);
            config.tex_null = dev->tex_null;
            config.circle_segment_count = 22;
            config.curve_segment_count = 22;
            config.arc_segment_count = 22;
            config.global_alpha = 1.0f;
            config.shape_AA = AA;
            config.line_AA = AA;

            /* setup buffers to load vertices and elements */
            {struct nk_buffer vbuf, ebuf;
            nk_buffer_init_fixed(&vbuf, vertices, MAX_VERTEX_MEMORY);
            nk_buffer_init_fixed(&ebuf, elements, MAX_ELEMENT_MEMORY);
            nk_convert(ctx, &dev->cmds, &vbuf, &ebuf, &config);}
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

      /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, ctx, &dev->cmds)
        {
            if (!cmd->elem_count) continue;
            glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            glScissor(
                (GLint)(cmd->clip_rect.x),
                (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h))),
                (GLint)(cmd->clip_rect.w),
                (GLint)(cmd->clip_rect.h));
            glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
            offset += cmd->elem_count;
        }
        nk_clear(ctx);
        nk_buffer_clear(&dev->cmds);
    }

    /* default OpenGL state */
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
}

void GameUI2D::pump_input(GLFWwindow *win){
    struct nk_context *ctx = &context;
    double x, y;
    nk_input_begin(ctx);
    glfwPollEvents();

    nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_SHIFT, 1);
    } else {
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glfwGetCursorPos(win, &x, &y);
    nk_input_motion(ctx, (int)x, (int)y);
    nk_input_button(ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    nk_input_end(ctx);
}



void GameUI2D::init(){

    //device_init(&device);
    const void *image; int w, h;
    struct nk_font *font;
    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    font = nk_font_atlas_add_default(&atlas, 13, 0);
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int)device.font_tex), &device.tex_null);
    nk_init_default(&context, &font->handle);
    glEnable(GL_TEXTURE_2D);
}




void GameUI2D::render(){
    struct nk_context *ctx = &context;
    enum {EASY, HARD};
    static int op = EASY;
    static float value = 0.6f;
    static int i =  20;

   
    // drawInventory(ctx);
    // this->uiRender = drawQuickInventory;
    // this->uiRender(ctx);
    this->draw(ctx);
    // this->uiRender(ctx);

    
     
    // testDraw(ctx);

    // if (nk_begin(ctx, "Show", nk_rect(400, 200, 320, 320),
    //         NK_WINDOW_BORDER|NK_WINDOW_MOVABLE)) {
    //         // fixed widget pixel width
    //         nk_layout_row_static(ctx, 30, 80, 1);
    //         if (nk_button_label(ctx, "button")) {
    //             // event handling
    //         }
            
    //         // fixed widget window ratio width
    //         nk_layout_row_dynamic(ctx, 30, 2);
    //         if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
    //         if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
    //         // custom widget pixel width
    //         nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
    //         {
    //             nk_layout_row_push(ctx, 50);
    //             nk_label(ctx, "Volume:", NK_TEXT_LEFT);
    //             nk_layout_row_push(ctx, 110);
                
    //             nk_slider_float(ctx, 0, &value, 1.0f, 0.1f);
    //         }
    //         nk_layout_row_end(ctx);
    //     }
    //     nk_end(ctx);
}

void GameUI2D::free(){
    nk_font_atlas_clear(&atlas);
    nk_free(&context);
    device_shutdown();
}

