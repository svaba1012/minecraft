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







// void testDraw(struct nk_context *ctx){
    
//      static int show_menu = nk_true;
//     static int titlebar = nk_true;
//     static int border = nk_true;
//     static int resize = nk_true;
//     static int movable = nk_true;
//     static int no_scrollbar = nk_false;
//     static int scale_left = nk_false;
//     static nk_flags window_flags = 0;
//     static int minimizable = nk_true;

//     /* popups */
//     static enum nk_style_header_align header_align = NK_HEADER_RIGHT;
//     static int show_app_about = nk_false;

//     /* window flags */
//     window_flags = 0;
//     ctx->style.window.header.align = header_align;
//     if (border) window_flags |= NK_WINDOW_BORDER;
//     if (resize) window_flags |= NK_WINDOW_SCALABLE;
//     if (movable) window_flags |= NK_WINDOW_MOVABLE;
//     if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
//     if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;
//     if (minimizable) window_flags |= NK_WINDOW_MINIMIZABLE;

//     if (nk_begin(ctx, "Overview", nk_rect(10, 10, 400, 600), window_flags))
//     {
//         if (show_menu)
//         {
//             /* menubar */
//             enum menu_states {MENU_DEFAULT, MENU_WINDOWS};
//             static nk_size mprog = 60;
//             static int mslider = 10;
//             static int mcheck = nk_true;
//             nk_menubar_begin(ctx);

//             /* menu #1 */
//             nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
//             nk_layout_row_push(ctx, 45);
//             if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
//             {
//                 static size_t prog = 40;
//                 static int slider = 10;
//                 static int check = nk_true;
//                 nk_layout_row_dynamic(ctx, 25, 1);
//                 if (nk_menu_item_label(ctx, "Hide", NK_TEXT_LEFT))
//                     show_menu = nk_false;
//                 if (nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
//                     show_app_about = nk_true;
//                 nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
//                 nk_slider_int(ctx, 0, &slider, 16, 1);
//                 nk_checkbox_label(ctx, "check", &check);
//                 nk_menu_end(ctx);
//             }
//             /* menu #2 */
//             nk_layout_row_push(ctx, 60);
//             if (nk_menu_begin_label(ctx, "ADVANCED", NK_TEXT_LEFT, nk_vec2(200, 600)))
//             {
//                 enum menu_state {MENU_NONE,MENU_FILE, MENU_EDIT,MENU_VIEW,MENU_CHART};
//                 static enum menu_state menu_state = MENU_NONE;
//                 enum nk_collapse_states state;

//                 state = (menu_state == MENU_FILE) ? NK_MAXIMIZED: NK_MINIMIZED;
//                 if (nk_tree_state_push(ctx, NK_TREE_TAB, "FILE", &state)) {
//                     menu_state = MENU_FILE;
//                     nk_menu_item_label(ctx, "New", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Close", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT);
//                     nk_tree_pop(ctx);
//                 } else menu_state = (menu_state == MENU_FILE) ? MENU_NONE: menu_state;

//                 state = (menu_state == MENU_EDIT) ? NK_MAXIMIZED: NK_MINIMIZED;
//                 if (nk_tree_state_push(ctx, NK_TREE_TAB, "EDIT", &state)) {
//                     menu_state = MENU_EDIT;
//                     nk_menu_item_label(ctx, "Copy", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Delete", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Cut", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Paste", NK_TEXT_LEFT);
//                     nk_tree_pop(ctx);
//                 } else menu_state = (menu_state == MENU_EDIT) ? MENU_NONE: menu_state;

//                 state = (menu_state == MENU_VIEW) ? NK_MAXIMIZED: NK_MINIMIZED;
//                 if (nk_tree_state_push(ctx, NK_TREE_TAB, "VIEW", &state)) {
//                     menu_state = MENU_VIEW;
//                     nk_menu_item_label(ctx, "About", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Options", NK_TEXT_LEFT);
//                     nk_menu_item_label(ctx, "Customize", NK_TEXT_LEFT);
//                     nk_tree_pop(ctx);
//                 } else menu_state = (menu_state == MENU_VIEW) ? MENU_NONE: menu_state;

//                 state = (menu_state == MENU_CHART) ? NK_MAXIMIZED: NK_MINIMIZED;
//                 if (nk_tree_state_push(ctx, NK_TREE_TAB, "CHART", &state)) {
//                     size_t i = 0;
//                     const float values[]={26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f,12.0f,8.0f,22.0f,28.0f};
//                     menu_state = MENU_CHART;
//                     nk_layout_row_dynamic(ctx, 150, 1);
//                     nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
//                     for (i = 0; i < NK_LEN(values); ++i)
//                         nk_chart_push(ctx, values[i]);
//                     nk_chart_end(ctx);
//                     nk_tree_pop(ctx);
//                 } else menu_state = (menu_state == MENU_CHART) ? MENU_NONE: menu_state;
//                 nk_menu_end(ctx);
//             }
//             /* menu widgets */
//             nk_layout_row_push(ctx, 70);
//             nk_progress(ctx, &mprog, 100, NK_MODIFIABLE);
//             nk_slider_int(ctx, 0, &mslider, 16, 1);
//             nk_checkbox_label(ctx, "check", &mcheck);
//             nk_menubar_end(ctx);
//         }

//         if (show_app_about)
//         {
//             /* about popup */
//             static struct nk_rect s = {20, 100, 300, 190};
//             if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
//             {
//                 nk_layout_row_dynamic(ctx, 20, 1);
//                 nk_label(ctx, "Nuklear", NK_TEXT_LEFT);
//                 nk_label(ctx, "By Micha Mettke", NK_TEXT_LEFT);
//                 nk_label(ctx, "nuklear is licensed under the public domain License.",  NK_TEXT_LEFT);
//                 nk_popup_end(ctx);
//             } else show_app_about = nk_false;
//         }

//         /* window flags */
//         if (nk_tree_push(ctx, NK_TREE_TAB, "Window", NK_MINIMIZED)) {
//             nk_layout_row_dynamic(ctx, 30, 2);
//             nk_checkbox_label(ctx, "Titlebar", &titlebar);
//             nk_checkbox_label(ctx, "Menu", &show_menu);
//             nk_checkbox_label(ctx, "Border", &border);
//             nk_checkbox_label(ctx, "Resizable", &resize);
//             nk_checkbox_label(ctx, "Movable", &movable);
//             nk_checkbox_label(ctx, "No Scrollbar", &no_scrollbar);
//             nk_checkbox_label(ctx, "Minimizable", &minimizable);
//             nk_checkbox_label(ctx, "Scale Left", &scale_left);
//             nk_tree_pop(ctx);
//         }

//         if (nk_tree_push(ctx, NK_TREE_TAB, "Widgets", NK_MINIMIZED))
//         {
//             enum options {A,B,C};
//             static int checkbox;
//             static int option;
//             if (nk_tree_push(ctx, NK_TREE_NODE, "Text", NK_MINIMIZED))
//             {
//                 /* Text Widgets */
//                 nk_layout_row_dynamic(ctx, 20, 1);
//                 nk_label(ctx, "Label aligned left", NK_TEXT_LEFT);
//                 nk_label(ctx, "Label aligned centered", NK_TEXT_CENTERED);
//                 nk_label(ctx, "Label aligned right", NK_TEXT_RIGHT);
//                 nk_label_colored(ctx, "Blue text", NK_TEXT_LEFT, nk_rgb(0,0,255));
//                 nk_label_colored(ctx, "Yellow text", NK_TEXT_LEFT, nk_rgb(255,255,0));
//                 nk_text(ctx, "Text without /0", 15, NK_TEXT_RIGHT);

//                 nk_layout_row_static(ctx, 100, 200, 1);
//                 nk_label_wrap(ctx, "This is a very long line to hopefully get this text to be wrapped into multiple lines to show line wrapping");
//                 nk_layout_row_dynamic(ctx, 100, 1);
//                 nk_label_wrap(ctx, "This is another long text to show dynamic window changes on multiline text");
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Button", NK_MINIMIZED))
//             {
//                 /* Buttons Widgets */
//                 nk_layout_row_static(ctx, 30, 100, 3);
//                 if (nk_button_label(ctx, "Button"))
//                     fprintf(stdout, "Button pressed!\n");
//                 nk_button_set_behavior(ctx, NK_BUTTON_REPEATER);
//                 if (nk_button_label(ctx, "Repeater"))
//                     fprintf(stdout, "Repeater is being pressed!\n");
//                 nk_button_set_behavior(ctx, NK_BUTTON_DEFAULT);
//                 nk_button_color(ctx, nk_rgb(0,0,255));

//                 nk_layout_row_static(ctx, 25, 25, 8);
//                 nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_SOLID);
//                 nk_button_symbol(ctx, NK_SYMBOL_CIRCLE_OUTLINE);
//                 nk_button_symbol(ctx, NK_SYMBOL_RECT_SOLID);
//                 nk_button_symbol(ctx, NK_SYMBOL_RECT_OUTLINE);
//                 nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_UP);
//                 nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_DOWN);
//                 nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT);
//                 nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT);

//                 nk_layout_row_static(ctx, 30, 100, 2);
//                 nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_LEFT, "prev", NK_TEXT_RIGHT);
//                 nk_button_symbol_label(ctx, NK_SYMBOL_TRIANGLE_RIGHT, "next", NK_TEXT_LEFT);
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Basic", NK_MINIMIZED))
//             {
//                 /* Basic widgets */
//                 static int int_slider = 5;
//                 static float float_slider = 2.5f;
//                 static size_t prog_value = 40;
//                 static float property_float = 2;
//                 static int property_int = 10;
//                 static int property_neg = 10;

//                 static float range_float_min = 0;
//                 static float range_float_max = 100;
//                 static float range_float_value = 50;
//                 static int range_int_min = 0;
//                 static int range_int_value = 2048;
//                 static int range_int_max = 4096;
//                 static const float ratio[] = {120, 150};

//                 nk_layout_row_static(ctx, 30, 100, 1);
//                 nk_checkbox_label(ctx, "Checkbox", &checkbox);

//                 nk_layout_row_static(ctx, 30, 80, 3);
//                 option = nk_option_label(ctx, "optionA", option == A) ? A : option;
//                 option = nk_option_label(ctx, "optionB", option == B) ? B : option;
//                 option = nk_option_label(ctx, "optionC", option == C) ? C : option;

//                 nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
//                 // nk_labelf(ctx, NK_TEXT_LEFT, "Slider int");
//                 nk_slider_int(ctx, 0, &int_slider, 10, 1);

//                 nk_label(ctx, "Slider float", NK_TEXT_LEFT);
//                 nk_slider_float(ctx, 0, &float_slider, 5.0, 0.5f);
//                 // nk_labelf(ctx, NK_TEXT_LEFT, "Progressbar: %zu" , prog_value);
//                 nk_progress(ctx, &prog_value, 100, NK_MODIFIABLE);

//                 nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
//                 nk_label(ctx, "Property float:", NK_TEXT_LEFT);
//                 nk_property_float(ctx, "Float:", 0, &property_float, 64.0f, 0.1f, 0.2f);
//                 nk_label(ctx, "Property int:", NK_TEXT_LEFT);
//                 nk_property_int(ctx, "Int:", 0, &property_int, 100.0f, 1, 1);
//                 nk_label(ctx, "Property neg:", NK_TEXT_LEFT);
//                 nk_property_int(ctx, "Neg:", -10, &property_neg, 10, 1, 1);

//                 nk_layout_row_dynamic(ctx, 25, 1);
//                 nk_label(ctx, "Range:", NK_TEXT_LEFT);
//                 nk_layout_row_dynamic(ctx, 25, 3);
//                 nk_property_float(ctx, "#min:", 0, &range_float_min, range_float_max, 1.0f, 0.2f);
//                 nk_property_float(ctx, "#float:", range_float_min, &range_float_value, range_float_max, 1.0f, 0.2f);
//                 nk_property_float(ctx, "#max:", range_float_min, &range_float_max, 100, 1.0f, 0.2f);

//                 nk_property_int(ctx, "#min:", INT_MIN, &range_int_min, range_int_max, 1, 10);
//                 nk_property_int(ctx, "#neg:", range_int_min, &range_int_value, range_int_max, 1, 10);
//                 nk_property_int(ctx, "#max:", range_int_min, &range_int_max, INT_MAX, 1, 10);

//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Inactive", NK_MINIMIZED))
//             {
//                 static int inactive = 1;
//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_checkbox_label(ctx, "Inactive", &inactive);

//                 nk_layout_row_static(ctx, 30, 80, 1);
//                 if (inactive) {
//                     struct nk_style_button button;
//                     button = ctx->style.button;
//                     ctx->style.button.normal = nk_style_item_color(nk_rgb(40,40,40));
//                     ctx->style.button.hover = nk_style_item_color(nk_rgb(40,40,40));
//                     ctx->style.button.active = nk_style_item_color(nk_rgb(40,40,40));
//                     ctx->style.button.border_color = nk_rgb(60,60,60);
//                     ctx->style.button.text_background = nk_rgb(60,60,60);
//                     ctx->style.button.text_normal = nk_rgb(60,60,60);
//                     ctx->style.button.text_hover = nk_rgb(60,60,60);
//                     ctx->style.button.text_active = nk_rgb(60,60,60);
//                     nk_button_label(ctx, "button");
//                     ctx->style.button = button;
//                 } else if (nk_button_label(ctx, "button"))
//                     fprintf(stdout, "button pressed\n");
//                 nk_tree_pop(ctx);
//             }


//             if (nk_tree_push(ctx, NK_TREE_NODE, "Selectable", NK_MINIMIZED))
//             {
//                 if (nk_tree_push(ctx, NK_TREE_NODE, "List", NK_MINIMIZED))
//                 {
//                     static int selected[4] = {nk_false, nk_false, nk_true, nk_false};
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[0]);
//                     nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[1]);
//                     nk_label(ctx, "Not Selectable", NK_TEXT_LEFT);
//                     nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[2]);
//                     nk_selectable_label(ctx, "Selectable", NK_TEXT_LEFT, &selected[3]);
//                     nk_tree_pop(ctx);
//                 }
//                 if (nk_tree_push(ctx, NK_TREE_NODE, "Grid", NK_MINIMIZED))
//                 {
//                     int i;
//                     static int selected[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
//                     nk_layout_row_static(ctx, 50, 50, 4);
//                     for (i = 0; i < 16; ++i) {
//                         if (nk_selectable_label(ctx, "Z", NK_TEXT_CENTERED, &selected[i])) {
//                             int x = (i % 4), y = i / 4;
//                             if (x > 0) selected[i - 1] ^= 1;
//                             if (x < 3) selected[i + 1] ^= 1;
//                             if (y > 0) selected[i - 4] ^= 1;
//                             if (y < 3) selected[i + 4] ^= 1;
//                         }
//                     }
//                     nk_tree_pop(ctx);
//                 }
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Combo", NK_MINIMIZED))
//             {
//                 /* Combobox Widgets
//                  * In this library comboboxes are not limited to being a popup
//                  * list of selectable text. Instead it is a abstract concept of
//                  * having something that is *selected* or displayed, a popup window
//                  * which opens if something needs to be modified and the content
//                  * of the popup which causes the *selected* or displayed value to
//                  * change or if wanted close the combobox.
//                  *
//                  * While strange at first handling comboboxes in a abstract way
//                  * solves the problem of overloaded window content. For example
//                  * changing a color value requires 4 value modifier (slider, property,...)
//                  * for RGBA then you need a label and ways to display the current color.
//                  * If you want to go fancy you even add rgb and hsv ratio boxes.
//                  * While fine for one color if you have a lot of them it because
//                  * tedious to look at and quite wasteful in space. You could add
//                  * a popup which modifies the color but this does not solve the
//                  * fact that it still requires a lot of cluttered space to do.
//                  *
//                  * In these kind of instance abstract comboboxes are quite handy. All
//                  * value modifiers are hidden inside the combobox popup and only
//                  * the color is shown if not open. This combines the clarity of the
//                  * popup with the ease of use of just using the space for modifiers.
//                  *
//                  * Other instances are for example time and especially date picker,
//                  * which only show the currently activated time/data and hide the
//                  * selection logic inside the combobox popup.
//                  */
//                 static float chart_selection = 8.0f;
//                 static int current_weapon = 0;
//                 static int check_values[5];
//                 static float position[3];
//                 static struct nk_color combo_color = {130, 50, 50, 255};
//                 static struct nk_colorf combo_color2 = {0.509f, 0.705f, 0.2f, 1.0f};
//                 static size_t prog_a =  20, prog_b = 40, prog_c = 10, prog_d = 90;
//                 static const char *weapons[] = {"Fist","Pistol","Shotgun","Plasma","BFG"};

//                 char buffer[64];
//                 size_t sum = 0;

//                 /* default combobox */
//                 nk_layout_row_static(ctx, 25, 200, 1);
//                 current_weapon = nk_combo(ctx, weapons, NK_LEN(weapons), current_weapon, 25, nk_vec2(200,200));

//                 /* slider color combobox */
//                 if (nk_combo_begin_color(ctx, combo_color, nk_vec2(200,200))) {
//                     float ratios[] = {0.15f, 0.85f};
//                     nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratios);
//                     nk_label(ctx, "R:", NK_TEXT_LEFT);
//                     combo_color.r = (nk_byte)nk_slide_int(ctx, 0, combo_color.r, 255, 5);
//                     nk_label(ctx, "G:", NK_TEXT_LEFT);
//                     combo_color.g = (nk_byte)nk_slide_int(ctx, 0, combo_color.g, 255, 5);
//                     nk_label(ctx, "B:", NK_TEXT_LEFT);
//                     combo_color.b = (nk_byte)nk_slide_int(ctx, 0, combo_color.b, 255, 5);
//                     nk_label(ctx, "A:", NK_TEXT_LEFT);
//                     combo_color.a = (nk_byte)nk_slide_int(ctx, 0, combo_color.a , 255, 5);
//                     nk_combo_end(ctx);
//                 }
//                 /* complex color combobox */
//                 if (nk_combo_begin_color(ctx, nk_rgb_cf(combo_color2), nk_vec2(200,400))) {
//                     enum color_mode {COL_RGB, COL_HSV};
//                     static int col_mode = COL_RGB;
//                     #ifndef DEMO_DO_NOT_USE_COLOR_PICKER
//                     nk_layout_row_dynamic(ctx, 120, 1);
//                     combo_color2 = nk_color_picker(ctx, combo_color2, NK_RGBA);
//                     #endif

//                     nk_layout_row_dynamic(ctx, 25, 2);
//                     col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
//                     col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

//                     nk_layout_row_dynamic(ctx, 25, 1);
//                     if (col_mode == COL_RGB) {
//                         combo_color2.r = nk_propertyf(ctx, "#R:", 0, combo_color2.r, 1.0f, 0.01f,0.005f);
//                         combo_color2.g = nk_propertyf(ctx, "#G:", 0, combo_color2.g, 1.0f, 0.01f,0.005f);
//                         combo_color2.b = nk_propertyf(ctx, "#B:", 0, combo_color2.b, 1.0f, 0.01f,0.005f);
//                         combo_color2.a = nk_propertyf(ctx, "#A:", 0, combo_color2.a, 1.0f, 0.01f,0.005f);
//                     } else {
//                         float hsva[4];
//                         nk_colorf_hsva_fv(hsva, combo_color2);
//                         hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f,0.05f);
//                         hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f,0.05f);
//                         hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f,0.05f);
//                         hsva[3] = nk_propertyf(ctx, "#A:", 0, hsva[3], 1.0f, 0.01f,0.05f);
//                         combo_color2 = nk_hsva_colorfv(hsva);
//                     }
//                     nk_combo_end(ctx);
//                 }
//                 /* progressbar combobox */
//                 sum = prog_a + prog_b + prog_c + prog_d;
//                 sprintf(buffer, "%lu", sum);
//                 if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
//                     nk_layout_row_dynamic(ctx, 30, 1);
//                     nk_progress(ctx, &prog_a, 100, NK_MODIFIABLE);
//                     nk_progress(ctx, &prog_b, 100, NK_MODIFIABLE);
//                     nk_progress(ctx, &prog_c, 100, NK_MODIFIABLE);
//                     nk_progress(ctx, &prog_d, 100, NK_MODIFIABLE);
//                     nk_combo_end(ctx);
//                 }

//                 /* checkbox combobox */
//                 sum = (size_t)(check_values[0] + check_values[1] + check_values[2] + check_values[3] + check_values[4]);
//                 sprintf(buffer, "%lu", sum);
//                 if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
//                     nk_layout_row_dynamic(ctx, 30, 1);
//                     nk_checkbox_label(ctx, weapons[0], &check_values[0]);
//                     nk_checkbox_label(ctx, weapons[1], &check_values[1]);
//                     nk_checkbox_label(ctx, weapons[2], &check_values[2]);
//                     nk_checkbox_label(ctx, weapons[3], &check_values[3]);
//                     nk_combo_end(ctx);
//                 }

//                 /* complex text combobox */
//                 sprintf(buffer, "%.2f, %.2f, %.2f", position[0], position[1],position[2]);
//                 if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,200))) {
//                     nk_layout_row_dynamic(ctx, 25, 1);
//                     nk_property_float(ctx, "#X:", -1024.0f, &position[0], 1024.0f, 1,0.5f);
//                     nk_property_float(ctx, "#Y:", -1024.0f, &position[1], 1024.0f, 1,0.5f);
//                     nk_property_float(ctx, "#Z:", -1024.0f, &position[2], 1024.0f, 1,0.5f);
//                     nk_combo_end(ctx);
//                 }

//                 /* chart combobox */
//                 sprintf(buffer, "%.1f", chart_selection);
//                 if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,250))) {
//                     size_t i = 0;
//                     static const float values[]={26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f, 12.0f, 8.0f, 22.0f, 28.0f, 5.0f};
//                     nk_layout_row_dynamic(ctx, 150, 1);
//                     nk_chart_begin(ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
//                     for (i = 0; i < NK_LEN(values); ++i) {
//                         nk_flags res = nk_chart_push(ctx, values[i]);
//                         if (res & NK_CHART_CLICKED) {
//                             chart_selection = values[i];
//                             nk_combo_close(ctx);
//                         }
//                     }
//                     nk_chart_end(ctx);
//                     nk_combo_end(ctx);
//                 }

//                 {
//                     static int time_selected = 0;
//                     static int date_selected = 0;
//                     static struct tm sel_time;
//                     static struct tm sel_date;
//                     if (!time_selected || !date_selected) {
//                         /* keep time and date updated if nothing is selected */
//                         time_t cur_time = time(0);
//                         struct tm *n = localtime(&cur_time);
//                         if (!time_selected)
//                             memcpy(&sel_time, n, sizeof(struct tm));
//                         if (!date_selected)
//                             memcpy(&sel_date, n, sizeof(struct tm));
//                     }

//                     /* time combobox */
//                     sprintf(buffer, "%02d:%02d:%02d", sel_time.tm_hour, sel_time.tm_min, sel_time.tm_sec);
//                     if (nk_combo_begin_label(ctx, buffer, nk_vec2(200,250))) {
//                         time_selected = 1;
//                         nk_layout_row_dynamic(ctx, 25, 1);
//                         sel_time.tm_sec = nk_propertyi(ctx, "#S:", 0, sel_time.tm_sec, 60, 1, 1);
//                         sel_time.tm_min = nk_propertyi(ctx, "#M:", 0, sel_time.tm_min, 60, 1, 1);
//                         sel_time.tm_hour = nk_propertyi(ctx, "#H:", 0, sel_time.tm_hour, 23, 1, 1);
//                         nk_combo_end(ctx);
//                     }

//                     /* date combobox */
//                     sprintf(buffer, "%02d-%02d-%02d", sel_date.tm_mday, sel_date.tm_mon+1, sel_date.tm_year+1900);
//                     if (nk_combo_begin_label(ctx, buffer, nk_vec2(350,400)))
//                     {
//                         int i = 0;
//                         const char *month[] = {"January", "February", "March",
//                             "April", "May", "June", "July", "August", "September",
//                             "October", "November", "December"};
//                         const char *week_days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
//                         const int month_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
//                         int year = sel_date.tm_year+1900;
//                         int leap_year = (!(year % 4) && ((year % 100))) || !(year % 400);
//                         int days = (sel_date.tm_mon == 1) ?
//                             month_days[sel_date.tm_mon] + leap_year:
//                             month_days[sel_date.tm_mon];

//                         /* header with month and year */
//                         date_selected = 1;
//                         nk_layout_row_begin(ctx, NK_DYNAMIC, 20, 3);
//                         nk_layout_row_push(ctx, 0.05f);
//                         if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_LEFT)) {
//                             if (sel_date.tm_mon == 0) {
//                                 sel_date.tm_mon = 11;
//                                 sel_date.tm_year = NK_MAX(0, sel_date.tm_year-1);
//                             } else sel_date.tm_mon--;
//                         }
//                         nk_layout_row_push(ctx, 0.9f);
//                         sprintf(buffer, "%s %d", month[sel_date.tm_mon], year);
//                         nk_label(ctx, buffer, NK_TEXT_CENTERED);
//                         nk_layout_row_push(ctx, 0.05f);
//                         if (nk_button_symbol(ctx, NK_SYMBOL_TRIANGLE_RIGHT)) {
//                             if (sel_date.tm_mon == 11) {
//                                 sel_date.tm_mon = 0;
//                                 sel_date.tm_year++;
//                             } else sel_date.tm_mon++;
//                         }
//                         nk_layout_row_end(ctx);

//                         /* good old week day formula (double because precision) */
//                         {int year_n = (sel_date.tm_mon < 2) ? year-1: year;
//                         int y = year_n % 100;
//                         int c = year_n / 100;
//                         int y4 = (int)((float)y / 4);
//                         int c4 = (int)((float)c / 4);
//                         int m = (int)(2.6 * (double)(((sel_date.tm_mon + 10) % 12) + 1) - 0.2);
//                         int week_day = (((1 + m + y + y4 + c4 - 2 * c) % 7) + 7) % 7;

//                         /* weekdays  */
//                         nk_layout_row_dynamic(ctx, 35, 7);
//                         for (i = 0; i < (int)NK_LEN(week_days); ++i)
//                             nk_label(ctx, week_days[i], NK_TEXT_CENTERED);

//                         /* days  */
//                         if (week_day > 0) nk_spacing(ctx, week_day);
//                         for (i = 1; i <= days; ++i) {
//                             sprintf(buffer, "%d", i);
//                             if (nk_button_label(ctx, buffer)) {
//                                 sel_date.tm_mday = i;
//                                 nk_combo_close(ctx);
//                             }
//                         }}
//                         nk_combo_end(ctx);
//                     }
//                 }

//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Input", NK_MINIMIZED))
//             {
//                 static const float ratio[] = {120, 150};
//                 static char field_buffer[64];
//                 static char text[9][64];
//                 static int text_len[9];
//                 static char box_buffer[512];
//                 static int field_len;
//                 static int box_len;
//                 nk_flags active;

//                 nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
//                 nk_label(ctx, "Default:", NK_TEXT_LEFT);

//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
//                 nk_label(ctx, "Int:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_decimal);
//                 nk_label(ctx, "Float:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_float);
//                 nk_label(ctx, "Hex:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[4], &text_len[4], 64, nk_filter_hex);
//                 nk_label(ctx, "Octal:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[5], &text_len[5], 64, nk_filter_oct);
//                 nk_label(ctx, "Binary:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_SIMPLE, text[6], &text_len[6], 64, nk_filter_binary);

//                 nk_label(ctx, "Password:", NK_TEXT_LEFT);
//                 {
//                     int i = 0;
//                     int old_len = text_len[8];
//                     char buffer[64];
//                     for (i = 0; i < text_len[8]; ++i) buffer[i] = '*';
//                     nk_edit_string(ctx, NK_EDIT_FIELD, buffer, &text_len[8], 64, nk_filter_default);
//                     if (old_len < text_len[8])
//                         memcpy(&text[8][old_len], &buffer[old_len], (nk_size)(text_len[8] - old_len));
//                 }

//                 nk_label(ctx, "Field:", NK_TEXT_LEFT);
//                 nk_edit_string(ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);

//                 nk_label(ctx, "Box:", NK_TEXT_LEFT);
//                 nk_layout_row_static(ctx, 180, 278, 1);
//                 nk_edit_string(ctx, NK_EDIT_BOX, box_buffer, &box_len, 512, nk_filter_default);

//                 nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
//                 active = nk_edit_string(ctx, NK_EDIT_FIELD|NK_EDIT_SIG_ENTER, text[7], &text_len[7], 64,  nk_filter_ascii);
//                 if (nk_button_label(ctx, "Submit") ||
//                     (active & NK_EDIT_COMMITED))
//                 {
//                     text[7][text_len[7]] = '\n';
//                     text_len[7]++;
//                     memcpy(&box_buffer[box_len], &text[7], (nk_size)text_len[7]);
//                     box_len += text_len[7];
//                     text_len[7] = 0;
//                 }
//                 nk_tree_pop(ctx);
//             }
//             nk_tree_pop(ctx);
//         }

//         if (nk_tree_push(ctx, NK_TREE_TAB, "Chart", NK_MINIMIZED))
//         {
//             /* Chart Widgets
//              * This library has two different rather simple charts. The line and the
//              * column chart. Both provide a simple way of visualizing values and
//              * have a retained mode and immediate mode API version. For the retain
//              * mode version `nk_plot` and `nk_plot_function` you either provide
//              * an array or a callback to call to handle drawing the graph.
//              * For the immediate mode version you start by calling `nk_chart_begin`
//              * and need to provide min and max values for scaling on the Y-axis.
//              * and then call `nk_chart_push` to push values into the chart.
//              * Finally `nk_chart_end` needs to be called to end the process. */
//             float id = 0;
//             static int col_index = -1;
//             static int line_index = -1;
//             float step = (2*3.141592654f) / 32;

//             int i;
//             int index = -1;
//             struct nk_rect bounds;

//             /* line chart */
//             id = 0;
//             index = -1;
//             nk_layout_row_dynamic(ctx, 100, 1);
//             bounds = nk_widget_bounds(ctx);
//             if (nk_chart_begin(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f)) {
//                 for (i = 0; i < 32; ++i) {
//                     nk_flags res = nk_chart_push(ctx, (float)cos(id));
//                     if (res & NK_CHART_HOVERING)
//                         index = (int)i;
//                     if (res & NK_CHART_CLICKED)
//                         line_index = (int)i;
//                     id += step;
//                 }
//                 nk_chart_end(ctx);
//             }

//             if (index != -1)
//                 // nk_tooltipf(ctx, "Value: %.2f", (float)cos((float)index*step));
//             if (line_index != -1) {
//                 nk_layout_row_dynamic(ctx, 20, 1);
//                 // nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)cos((float)index*step));
//             }

//             /* column chart */
//             nk_layout_row_dynamic(ctx, 100, 1);
//             bounds = nk_widget_bounds(ctx);
//             if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
//                 for (i = 0; i < 32; ++i) {
//                     nk_flags res = nk_chart_push(ctx, (float)fabs(sin(id)));
//                     if (res & NK_CHART_HOVERING)
//                         index = (int)i;
//                     if (res & NK_CHART_CLICKED)
//                         col_index = (int)i;
//                     id += step;
//                 }
//                 nk_chart_end(ctx);
//             }
//             if (index != -1)
//                 // nk_tooltipf(ctx, "Value: %.2f", (float)fabs(sin(step * (float)index)));
//             if (col_index != -1) {
//                 nk_layout_row_dynamic(ctx, 20, 1);
//                 // nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)fabs(sin(step * (float)col_index)));
//             }

//             /* mixed chart */
//             nk_layout_row_dynamic(ctx, 100, 1);
//             bounds = nk_widget_bounds(ctx);
//             if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
//                 nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
//                 nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
//                 for (id = 0, i = 0; i < 32; ++i) {
//                     nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
//                     nk_chart_push_slot(ctx, (float)cos(id), 1);
//                     nk_chart_push_slot(ctx, (float)sin(id), 2);
//                     id += step;
//                 }
//             }
//             nk_chart_end(ctx);

//             /* mixed colored chart */
//             nk_layout_row_dynamic(ctx, 100, 1);
//             bounds = nk_widget_bounds(ctx);
//             if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
//                 nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
//                 nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, -1.0f, 1.0f);
//                 for (id = 0, i = 0; i < 32; ++i) {
//                     nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
//                     nk_chart_push_slot(ctx, (float)cos(id), 1);
//                     nk_chart_push_slot(ctx, (float)sin(id), 2);
//                     id += step;
//                 }
//             }
//             nk_chart_end(ctx);
//             nk_tree_pop(ctx);
//         }

//         if (nk_tree_push(ctx, NK_TREE_TAB, "Popup", NK_MINIMIZED))
//         {
//             static struct nk_color color = {255,0,0, 255};
//             static int select[4];
//             static int popup_active;
//             const struct nk_input *in = &ctx->input;
//             struct nk_rect bounds;

//             /* menu contextual */
//             nk_layout_row_static(ctx, 30, 160, 1);
//             bounds = nk_widget_bounds(ctx);
//             nk_label(ctx, "Right click me for menu", NK_TEXT_LEFT);

//             if (nk_contextual_begin(ctx, 0, nk_vec2(100, 300), bounds)) {
//                 static size_t prog = 40;
//                 static int slider = 10;

//                 nk_layout_row_dynamic(ctx, 25, 1);
//                 nk_checkbox_label(ctx, "Menu", &show_menu);
//                 nk_progress(ctx, &prog, 100, NK_MODIFIABLE);
//                 nk_slider_int(ctx, 0, &slider, 16, 1);
//                 if (nk_contextual_item_label(ctx, "About", NK_TEXT_CENTERED))
//                     show_app_about = nk_true;
//                 nk_selectable_label(ctx, select[0]?"Unselect":"Select", NK_TEXT_LEFT, &select[0]);
//                 nk_selectable_label(ctx, select[1]?"Unselect":"Select", NK_TEXT_LEFT, &select[1]);
//                 nk_selectable_label(ctx, select[2]?"Unselect":"Select", NK_TEXT_LEFT, &select[2]);
//                 nk_selectable_label(ctx, select[3]?"Unselect":"Select", NK_TEXT_LEFT, &select[3]);
//                 nk_contextual_end(ctx);
//             }

//             /* color contextual */
//             nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
//             nk_layout_row_push(ctx, 100);
//             nk_label(ctx, "Right Click here:", NK_TEXT_LEFT);
//             nk_layout_row_push(ctx, 50);
//             bounds = nk_widget_bounds(ctx);
//             nk_button_color(ctx, color);
//             nk_layout_row_end(ctx);

//             if (nk_contextual_begin(ctx, 0, nk_vec2(350, 60), bounds)) {
//                 nk_layout_row_dynamic(ctx, 30, 4);
//                 color.r = (nk_byte)nk_propertyi(ctx, "#r", 0, color.r, 255, 1, 1);
//                 color.g = (nk_byte)nk_propertyi(ctx, "#g", 0, color.g, 255, 1, 1);
//                 color.b = (nk_byte)nk_propertyi(ctx, "#b", 0, color.b, 255, 1, 1);
//                 color.a = (nk_byte)nk_propertyi(ctx, "#a", 0, color.a, 255, 1, 1);
//                 nk_contextual_end(ctx);
//             }

//             /* popup */
//             nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
//             nk_layout_row_push(ctx, 100);
//             nk_label(ctx, "Popup:", NK_TEXT_LEFT);
//             nk_layout_row_push(ctx, 50);
//             if (nk_button_label(ctx, "Popup"))
//                 popup_active = 1;
//             nk_layout_row_end(ctx);

//             if (popup_active)
//             {
//                 static struct nk_rect s = {20, 100, 220, 90};
//                 if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", 0, s))
//                 {
//                     nk_layout_row_dynamic(ctx, 25, 1);
//                     nk_label(ctx, "A terrible error as occured", NK_TEXT_LEFT);
//                     nk_layout_row_dynamic(ctx, 25, 2);
//                     if (nk_button_label(ctx, "OK")) {
//                         popup_active = 0;
//                         nk_popup_close(ctx);
//                     }
//                     if (nk_button_label(ctx, "Cancel")) {
//                         popup_active = 0;
//                         nk_popup_close(ctx);
//                     }
//                     nk_popup_end(ctx);
//                 } else popup_active = nk_false;
//             }

//             /* tooltip */
//             nk_layout_row_static(ctx, 30, 150, 1);
//             bounds = nk_widget_bounds(ctx);
//             nk_label(ctx, "Hover me for tooltip", NK_TEXT_LEFT);
//             if (nk_input_is_mouse_hovering_rect(in, bounds))
//                 nk_tooltip(ctx, "This is a tooltip");

//             nk_tree_pop(ctx);
//         }

//         if (nk_tree_push(ctx, NK_TREE_TAB, "Layout", NK_MINIMIZED))
//         {
//             if (nk_tree_push(ctx, NK_TREE_NODE, "Widget", NK_MINIMIZED))
//             {
//                 float ratio_two[] = {0.2f, 0.6f, 0.2f};
//                 float width_two[] = {100, 200, 50};

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Dynamic fixed column layout with generated position and size:", NK_TEXT_LEFT);
//                 nk_layout_row_dynamic(ctx, 60, 6);
//                 struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
                
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
//                 inventory_box(ctx, image);
                
                
                

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "static fixed column layout with generated position and size:", NK_TEXT_LEFT);
//                 nk_layout_row_static(ctx, 30, 100, 3);
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Dynamic array-based custom column layout with generated position and custom size:",NK_TEXT_LEFT);
//                 nk_layout_row(ctx, NK_DYNAMIC, 30, 3, ratio_two);
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Static array-based custom column layout with generated position and custom size:",NK_TEXT_LEFT );
//                 nk_layout_row(ctx, NK_STATIC, 30, 3, width_two);
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Dynamic immediate mode custom column layout with generated position and custom size:",NK_TEXT_LEFT);
//                 nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 3);
//                 nk_layout_row_push(ctx, 0.2f);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_push(ctx, 0.6f);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_push(ctx, 0.2f);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_end(ctx);

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Static immediate mode custom column layout with generated position and custom size:", NK_TEXT_LEFT);
//                 nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
//                 nk_layout_row_push(ctx, 100);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_push(ctx, 200);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_push(ctx, 50);
//                 nk_button_label(ctx, "button");
//                 nk_layout_row_end(ctx);

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Static free space with custom position and custom size:", NK_TEXT_LEFT);
//                 nk_layout_space_begin(ctx, NK_STATIC, 60, 4);
//                 nk_layout_space_push(ctx, nk_rect(100, 0, 100, 30));
//                 nk_button_label(ctx, "button");
//                 nk_layout_space_push(ctx, nk_rect(0, 15, 100, 30));
//                 nk_button_label(ctx, "button");
//                 nk_layout_space_push(ctx, nk_rect(200, 15, 100, 30));
//                 nk_button_label(ctx, "button");
//                 nk_layout_space_push(ctx, nk_rect(100, 30, 100, 30));
//                 nk_button_label(ctx, "button");
//                 nk_layout_space_end(ctx);

//                 nk_layout_row_dynamic(ctx, 30, 1);
//                 nk_label(ctx, "Row template:", NK_TEXT_LEFT);
//                 nk_layout_row_template_begin(ctx, 30);
//                 nk_layout_row_template_push_dynamic(ctx);
//                 nk_layout_row_template_push_variable(ctx, 80);
//                 nk_layout_row_template_push_static(ctx, 80);
//                 nk_layout_row_template_end(ctx);
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");
//                 nk_button_label(ctx, "button");

//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Group", NK_MINIMIZED))
//             {
//                 static int group_titlebar = nk_false;
//                 static int group_border = nk_true;
//                 static int group_no_scrollbar = nk_false;
//                 static int group_width = 320;
//                 static int group_height = 200;

//                 nk_flags group_flags = 0;
//                 if (group_border) group_flags |= NK_WINDOW_BORDER;
//                 if (group_no_scrollbar) group_flags |= NK_WINDOW_NO_SCROLLBAR;
//                 if (group_titlebar) group_flags |= NK_WINDOW_TITLE;

//                 nk_layout_row_dynamic(ctx, 30, 3);
//                 nk_checkbox_label(ctx, "Titlebar", &group_titlebar);
//                 nk_checkbox_label(ctx, "Border", &group_border);
//                 nk_checkbox_label(ctx, "No Scrollbar", &group_no_scrollbar);

//                 nk_layout_row_begin(ctx, NK_STATIC, 22, 3);
//                 nk_layout_row_push(ctx, 50);
//                 nk_label(ctx, "size:", NK_TEXT_LEFT);
//                 nk_layout_row_push(ctx, 130);
//                 nk_property_int(ctx, "#Width:", 100, &group_width, 500, 10, 1);
//                 nk_layout_row_push(ctx, 130);
//                 nk_property_int(ctx, "#Height:", 100, &group_height, 500, 10, 1);
//                 nk_layout_row_end(ctx);

//                 nk_layout_row_static(ctx, (float)group_height, group_width, 2);
//                 if (nk_group_begin(ctx, "Group", group_flags)) {
//                     int i = 0;
//                     static int selected[16];
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 0; i < 16; ++i)
//                         nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
//                     nk_group_end(ctx);
//                 }
//                 nk_tree_pop(ctx);
//             }
//             if (nk_tree_push(ctx, NK_TREE_NODE, "Tree", NK_MINIMIZED))
//             {
//                 static int root_selected = 0;
//                 int sel = root_selected;
//                 if (nk_tree_element_push(ctx, NK_TREE_NODE, "Root", NK_MINIMIZED, &sel)) {
//                     static int selected[8];
//                     int i = 0, node_select = selected[0];
//                     if (sel != root_selected) {
//                         root_selected = sel;
//                         for (i = 0; i < 8; ++i)
//                             selected[i] = sel;
//                     }
//                     if (nk_tree_element_push(ctx, NK_TREE_NODE, "Node", NK_MINIMIZED, &node_select)) {
//                         int j = 0;
//                         static int sel_nodes[4];
//                         if (node_select != selected[0]) {
//                             selected[0] = node_select;
//                             for (i = 0; i < 4; ++i)
//                                 sel_nodes[i] = node_select;
//                         }
//                         nk_layout_row_static(ctx, 18, 100, 1);
//                         for (j = 0; j < 4; ++j)
//                             nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (sel_nodes[j]) ? "Selected": "Unselected", NK_TEXT_RIGHT, &sel_nodes[j]);
//                         nk_tree_element_pop(ctx);
//                     }
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 1; i < 8; ++i)
//                         nk_selectable_symbol_label(ctx, NK_SYMBOL_CIRCLE_SOLID, (selected[i]) ? "Selected": "Unselected", NK_TEXT_RIGHT, &selected[i]);
//                     nk_tree_element_pop(ctx);
//                 }
//                 nk_tree_pop(ctx);
//             }
//             if (nk_tree_push(ctx, NK_TREE_NODE, "Notebook", NK_MINIMIZED))
//             {
//                 static int current_tab = 0;
//                 struct nk_rect bounds;
//                 float step = (2*3.141592654f) / 32;
//                 enum chart_type {CHART_LINE, CHART_HISTO, CHART_MIXED};
//                 const char *names[] = {"Lines", "Columns", "Mixed"};
//                 float id = 0;
//                 int i;

//                 /* Header */
//                 nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0,0));
//                 nk_style_push_float(ctx, &ctx->style.button.rounding, 0);
//                 nk_layout_row_begin(ctx, NK_STATIC, 20, 3);
//                 for (i = 0; i < 3; ++i) {
//                     /* make sure button perfectly fits text */
//                     const struct nk_user_font *f = ctx->style.font;
//                     float text_width = f->width(f->userdata, f->height, names[i], nk_strlen(names[i]));
//                     float widget_width = text_width + 3 * ctx->style.button.padding.x;
//                     nk_layout_row_push(ctx, widget_width);
//                     if (current_tab == i) {
//                         /* active tab gets highlighted */
//                         struct nk_style_item button_color = ctx->style.button.normal;
//                         ctx->style.button.normal = ctx->style.button.active;
//                         current_tab = nk_button_label(ctx, names[i]) ? i: current_tab;
//                         ctx->style.button.normal = button_color;
//                     } else current_tab = nk_button_label(ctx, names[i]) ? i: current_tab;
//                 }
//                 nk_style_pop_float(ctx);

//                 /* Body */
//                 nk_layout_row_dynamic(ctx, 140, 1);
//                 if (nk_group_begin(ctx, "Notebook", NK_WINDOW_BORDER))
//                 {
//                     nk_style_pop_vec2(ctx);
//                     switch (current_tab) {
//                     default: break;
//                     case CHART_LINE:
//                         nk_layout_row_dynamic(ctx, 100, 1);
//                         bounds = nk_widget_bounds(ctx);
//                         if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
//                             nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
//                             for (i = 0, id = 0; i < 32; ++i) {
//                                 nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
//                                 nk_chart_push_slot(ctx, (float)cos(id), 1);
//                                 id += step;
//                             }
//                         }
//                         nk_chart_end(ctx);
//                         break;
//                     case CHART_HISTO:
//                         nk_layout_row_dynamic(ctx, 100, 1);
//                         bounds = nk_widget_bounds(ctx);
//                         if (nk_chart_begin_colored(ctx, NK_CHART_COLUMN, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
//                             for (i = 0, id = 0; i < 32; ++i) {
//                                 nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
//                                 id += step;
//                             }
//                         }
//                         nk_chart_end(ctx);
//                         break;
//                     case CHART_MIXED:
//                         nk_layout_row_dynamic(ctx, 100, 1);
//                         bounds = nk_widget_bounds(ctx);
//                         if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
//                             nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
//                             nk_chart_add_slot_colored(ctx, NK_CHART_COLUMN, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, 0.0f, 1.0f);
//                             for (i = 0, id = 0; i < 32; ++i) {
//                                 nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
//                                 nk_chart_push_slot(ctx, (float)fabs(cos(id)), 1);
//                                 nk_chart_push_slot(ctx, (float)fabs(sin(id)), 2);
//                                 id += step;
//                             }
//                         }
//                         nk_chart_end(ctx);
//                         break;
//                     }
//                     nk_group_end(ctx);
//                 } else nk_style_pop_vec2(ctx);
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Simple", NK_MINIMIZED))
//             {
//                 nk_layout_row_dynamic(ctx, 300, 2);
//                 if (nk_group_begin(ctx, "Group_Without_Border", 0)) {
//                     int i = 0;
//                     char buffer[64];
//                     nk_layout_row_static(ctx, 18, 150, 1);
//                     for (i = 0; i < 64; ++i) {
//                         sprintf(buffer, "0x%02x", i);
//                         // nk_labelf(ctx, NK_TEXT_LEFT, "%s: scrollable region", buffer);
//                     }
//                     nk_group_end(ctx);
//                 }
//                 if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
//                     int i = 0;
//                     char buffer[64];
//                     nk_layout_row_dynamic(ctx, 25, 2);
//                     for (i = 0; i < 64; ++i) {
//                         sprintf(buffer, "%08d", ((((i%7)*10)^32))+(64+(i%2)*2));
//                         nk_button_label(ctx, buffer);
//                     }
//                     nk_group_end(ctx);
//                 }
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Complex", NK_MINIMIZED))
//             {
//                 int i;
//                 nk_layout_space_begin(ctx, NK_STATIC, 500, 64);
//                 nk_layout_space_push(ctx, nk_rect(0,0,150,500));
//                 if (nk_group_begin(ctx, "Group_left", NK_WINDOW_BORDER)) {
//                     static int selected[32];
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 0; i < 32; ++i)
//                         nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
//                     nk_group_end(ctx);
//                 }

//                 nk_layout_space_push(ctx, nk_rect(160,0,150,240));
//                 if (nk_group_begin(ctx, "Group_top", NK_WINDOW_BORDER)) {
//                     nk_layout_row_dynamic(ctx, 25, 1);
//                     nk_button_label(ctx, "#FFAA");
//                     nk_button_label(ctx, "#FFBB");
//                     nk_button_label(ctx, "#FFCC");
//                     nk_button_label(ctx, "#FFDD");
//                     nk_button_label(ctx, "#FFEE");
//                     nk_button_label(ctx, "#FFFF");
//                     nk_group_end(ctx);
//                 }

//                 nk_layout_space_push(ctx, nk_rect(160,250,150,250));
//                 if (nk_group_begin(ctx, "Group_buttom", NK_WINDOW_BORDER)) {
//                     nk_layout_row_dynamic(ctx, 25, 1);
//                     nk_button_label(ctx, "#FFAA");
//                     nk_button_label(ctx, "#FFBB");
//                     nk_button_label(ctx, "#FFCC");
//                     nk_button_label(ctx, "#FFDD");
//                     nk_button_label(ctx, "#FFEE");
//                     nk_button_label(ctx, "#FFFF");
//                     nk_group_end(ctx);
//                 }

//                 nk_layout_space_push(ctx, nk_rect(320,0,150,150));
//                 if (nk_group_begin(ctx, "Group_right_top", NK_WINDOW_BORDER)) {
//                     static int selected[4];
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 0; i < 4; ++i)
//                         nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
//                     nk_group_end(ctx);
//                 }

//                 nk_layout_space_push(ctx, nk_rect(320,160,150,150));
//                 if (nk_group_begin(ctx, "Group_right_center", NK_WINDOW_BORDER)) {
//                     static int selected[4];
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 0; i < 4; ++i)
//                         nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
//                     nk_group_end(ctx);
//                 }

//                 nk_layout_space_push(ctx, nk_rect(320,320,150,150));
//                 if (nk_group_begin(ctx, "Group_right_bottom", NK_WINDOW_BORDER)) {
//                     static int selected[4];
//                     nk_layout_row_static(ctx, 18, 100, 1);
//                     for (i = 0; i < 4; ++i)
//                         nk_selectable_label(ctx, (selected[i]) ? "Selected": "Unselected", NK_TEXT_CENTERED, &selected[i]);
//                     nk_group_end(ctx);
//                 }
//                 nk_layout_space_end(ctx);
//                 nk_tree_pop(ctx);
//             }

//             if (nk_tree_push(ctx, NK_TREE_NODE, "Splitter", NK_MINIMIZED))
//             {
//                 const struct nk_input *in = &ctx->input;
//                 nk_layout_row_static(ctx, 20, 320, 1);
//                 nk_label(ctx, "Use slider and spinner to change tile size", NK_TEXT_LEFT);
//                 nk_label(ctx, "Drag the space between tiles to change tile ratio", NK_TEXT_LEFT);

//                 if (nk_tree_push(ctx, NK_TREE_NODE, "Vertical", NK_MINIMIZED))
//                 {
//                     static float a = 100, b = 100, c = 100;
//                     struct nk_rect bounds;

//                     float row_layout[5];
//                     row_layout[0] = a;
//                     row_layout[1] = 8;
//                     row_layout[2] = b;
//                     row_layout[3] = 8;
//                     row_layout[4] = c;

//                     /* header */
//                     nk_layout_row_static(ctx, 30, 100, 2);
//                     nk_label(ctx, "left:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

//                     nk_label(ctx, "middle:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

//                     nk_label(ctx, "right:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

//                     /* tiles */
//                     nk_layout_row(ctx, NK_STATIC, 200, 5, row_layout);

//                     /* left space */
//                     if (nk_group_begin(ctx, "left", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
//                         nk_layout_row_dynamic(ctx, 25, 1);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }

//                     /* scaler */
//                     bounds = nk_widget_bounds(ctx);
//                     nk_spacing(ctx, 1);
//                     if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
//                         nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
//                         nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
//                     {
//                         a = row_layout[0] + in->mouse.delta.x;
//                         b = row_layout[2] - in->mouse.delta.x;
//                     }

//                     /* middle space */
//                     if (nk_group_begin(ctx, "center", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
//                         nk_layout_row_dynamic(ctx, 25, 1);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }

//                     /* scaler */
//                     bounds = nk_widget_bounds(ctx);
//                     nk_spacing(ctx, 1);
//                     if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
//                         nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
//                         nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
//                     {
//                         b = (row_layout[2] + in->mouse.delta.x);
//                         c = (row_layout[4] - in->mouse.delta.x);
//                     }

//                     /* right space */
//                     if (nk_group_begin(ctx, "right", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
//                         nk_layout_row_dynamic(ctx, 25, 1);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }

//                     nk_tree_pop(ctx);
//                 }

//                 if (nk_tree_push(ctx, NK_TREE_NODE, "Horizontal", NK_MINIMIZED))
//                 {
//                     static float a = 100, b = 100, c = 100;
//                     struct nk_rect bounds;

//                     /* header */
//                     nk_layout_row_static(ctx, 30, 100, 2);
//                     nk_label(ctx, "top:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &a, 200.0f, 10.0f);

//                     nk_label(ctx, "middle:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &b, 200.0f, 10.0f);

//                     nk_label(ctx, "bottom:", NK_TEXT_LEFT);
//                     nk_slider_float(ctx, 10.0f, &c, 200.0f, 10.0f);

//                     /* top space */
//                     nk_layout_row_dynamic(ctx, a, 1);
//                     if (nk_group_begin(ctx, "top", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
//                         nk_layout_row_dynamic(ctx, 25, 3);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }

//                     /* scaler */
//                     nk_layout_row_dynamic(ctx, 8, 1);
//                     bounds = nk_widget_bounds(ctx);
//                     nk_spacing(ctx, 1);
//                     if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
//                         nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
//                         nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
//                     {
//                         a = a + in->mouse.delta.y;
//                         b = b - in->mouse.delta.y;
//                     }

//                     /* middle space */
//                     nk_layout_row_dynamic(ctx, b, 1);
//                     if (nk_group_begin(ctx, "middle", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
//                         nk_layout_row_dynamic(ctx, 25, 3);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }

//                     {
//                         /* scaler */
//                         nk_layout_row_dynamic(ctx, 8, 1);
//                         bounds = nk_widget_bounds(ctx);
//                         if ((nk_input_is_mouse_hovering_rect(in, bounds) ||
//                             nk_input_is_mouse_prev_hovering_rect(in, bounds)) &&
//                             nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
//                         {
//                             b = b + in->mouse.delta.y;
//                             c = c - in->mouse.delta.y;
//                         }
//                     }

//                     /* bottom space */
//                     nk_layout_row_dynamic(ctx, c, 1);
//                     if (nk_group_begin(ctx, "bottom", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER)) {
//                         nk_layout_row_dynamic(ctx, 25, 3);
//                         nk_button_label(ctx, "#FFAA");
//                         nk_button_label(ctx, "#FFBB");
//                         nk_button_label(ctx, "#FFCC");
//                         nk_button_label(ctx, "#FFDD");
//                         nk_button_label(ctx, "#FFEE");
//                         nk_button_label(ctx, "#FFFF");
//                         nk_group_end(ctx);
//                     }
//                     nk_tree_pop(ctx);
//                 }
//                 nk_tree_pop(ctx);
//             }
//             nk_tree_pop(ctx);
//         }
//     }
//     nk_end(ctx);
// }

// struct nk_image icon_load(const char *filename){
//     int x,y,n;
//     GLuint tex;
//     unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//     if (!data) printf("[SDL]: failed to load image: %s", filename);

//     glGenTextures(1, &tex);
//     glBindTexture(GL_TEXTURE_2D, tex);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//     glGenerateMipmap(GL_TEXTURE_2D);
//     stbi_image_free(data);
//     return nk_image_id((int)tex);
// }

// void inventory_box( struct nk_context *ctx, struct nk_image image){
//     // struct nk_context *ctx = &context;
//     struct nk_style_window old_style = ctx->style.window;
//     ctx->style.window.group_padding = nk_vec2(0, 0);
//     if (nk_group_begin(ctx, "Group", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
//         // nk_layout_row_dynamic(ctx, 50, 1);
//         nk_layout_row_static(ctx, 50, 50, 1);
//         nk_image(ctx, image);
//         nk_group_end(ctx);
//     }
//     ctx->style.window = old_style;
// }

// void drawInventory(struct nk_context *ctx){
//     const int INVENTORY_ITEM_SIZE = 50;
//     const int xPos = 10, yPos = 10;
//     if (nk_begin(ctx, "Overview", nk_rect(xPos, yPos, 10.1 * INVENTORY_ITEM_SIZE, 10 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
//         struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
//         float rowWidths[6] = {1, 3, 1, 2.2, 1, 1};
//         for(int i = 0; i < 6; i++){
//             rowWidths[i] *= INVENTORY_ITEM_SIZE;
//         }

//         nk_layout_row(ctx, NK_STATIC, 4.7 * INVENTORY_ITEM_SIZE, 6, rowWidths);
//         struct nk_style_window old_style = ctx->style.window;
//         ctx->style.window.group_padding = nk_vec2(0, 0);
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             // nk_layout_row_dynamic(ctx, 50, 1);
//             nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);
//             nk_group_end(ctx);
//         }
//         ctx->style.window = old_style;
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_group_end(ctx);
//         }

//         old_style = ctx->style.window;
//         ctx->style.window.group_padding = nk_vec2(0, 0);
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
//             if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//                 nk_group_end(ctx);
//             }
//             if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//                 nk_group_end(ctx);
//             }
//             if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//                 nk_group_end(ctx);
//             }
//             inventory_box(ctx, image);
//             nk_group_end(ctx);
//         }
//         ctx->style.window = old_style;
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.3, 1);
//             nk_label(ctx, "Crafting", NK_TEXT_LEFT);
//             nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 2);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);
//             inventory_box(ctx, image);

//             // ! recepies button
//             nk_group_end(ctx);
//         }

//         old_style = ctx->style.window;
//         ctx->style.window.group_padding = nk_vec2(0, 0);
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
//             if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//                 nk_group_end(ctx);
//             }
//             nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
//             inventory_box(ctx, image);
            
//             nk_group_end(ctx);
//         }
//         ctx->style.window = old_style;
//         old_style = ctx->style.window;

//         ctx->style.window.group_padding = nk_vec2(0, 0);
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
//             if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//                 nk_group_end(ctx);
//             }
//             nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
//             inventory_box(ctx, image);
            
//             nk_group_end(ctx);
//         }
//         ctx->style.window = old_style;

//         nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
//         for(int i = 0; i < 3; i++){
//             for(int j = 0; j < 9; j++){
//                 inventory_box(ctx, image);
//             }
//         }
//         nk_layout_row_dynamic(ctx, 0.2 * INVENTORY_ITEM_SIZE, 1);
//         if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
//             nk_group_end(ctx);
//         }
//         nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
//         for(int j = 0; j < 9; j++){
//             inventory_box(ctx, image);
//         }

//         // inventory_box(ctx, image);
//     }
//     nk_end(ctx);
// }

// void drawQuickInventory(struct nk_context *ctx){
//     const int INVENTORY_ITEM_SIZE = 50;
//     const int xPos = 130, yPos = 520;
//     if (nk_begin(ctx, "Overview", nk_rect(xPos, yPos, 9.9 * INVENTORY_ITEM_SIZE, 1.25 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
//         struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
//         nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
//         for(int j = 0; j < 9; j++){
//             inventory_box(ctx, image);
//         }
//     }
//     nk_end(ctx);
// }


void GameUI2D::draw(){
    struct nk_context *ctx = &context;
    enum {EASY, HARD};
    static int op = EASY;
    static float value = 0.6f;
    static int i =  20;

   
    // drawInventory(ctx);
    // this->uiRender = drawQuickInventory;
    this->uiRender(ctx);
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

