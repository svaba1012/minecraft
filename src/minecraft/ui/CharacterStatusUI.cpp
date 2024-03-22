#include "minecraft/ui/CharacterStatusUI.hpp"

#define ITEM_SIZE 23
#define ITEM_COUNT 10

using namespace std;

CharacterStatusUI::CharacterStatusUI(){

}

struct nk_image icon_load2(const char *filename){
    int x,y,n;
    GLuint tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data) printf("[SDL]: failed to load image: %s", filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int)tex);
}

void CharacterStatusUI::draw(struct nk_context* ctx){
    // const int INVENTORY_ITEM_SIZE = 50;
    const int xPos = 130, yPos = 450;
    struct nk_style *s = &ctx->style;
    nk_style_push_color(ctx, &s->window.background, nk_rgba(0,0,0,0));
    nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0)));
    if (nk_begin(ctx, "CharStatus", nk_rect(xPos, yPos, 22 * ITEM_SIZE, 3.5 * ITEM_SIZE), NK_WINDOW_NO_SCROLLBAR)){
        float armor = Character::instance->getArmor();
        float health = Character::instance->health;
        float hunger = Character::instance->hunger;
        float expLvl = Character::instance->expLvl;

        struct nk_image imageArmorFull = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/armor_full.png");
        struct nk_image imageArmorHalf = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/armor_half.png");
        struct nk_image imageArmorEmpty = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/armor_empty.png");
        
        nk_layout_space_begin(ctx, NK_STATIC, ITEM_SIZE, INT_MAX);
        // nk_layout_row_static(ctx, ITEM_SIZE, ITEM_SIZE, ITEM_COUNT);
        if(armor > 0){
            for(int i = 0; i < ITEM_COUNT; i++){
                nk_layout_space_push(ctx, nk_rect(i * ITEM_SIZE,0,ITEM_SIZE,ITEM_SIZE));
                if(armor - i >= 1){
                    nk_image(ctx, imageArmorFull);
                }else if(armor - i > 0.1){
                    nk_image(ctx, imageArmorHalf);
                }else{
                    nk_image(ctx, imageArmorEmpty);
                }
            }
        }
        
        nk_layout_space_end(ctx);
        struct nk_image imageHealthFull = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/heart/full.png");
        struct nk_image imageHealthHalf = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/heart/half.png");
        struct nk_image imageHealthEmpty = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/heart/container_hardcore.png");

        nk_layout_space_begin(ctx, NK_STATIC, ITEM_SIZE, INT_MAX);
        
        // nk_layout_row_static(ctx, ITEM_SIZE, ITEM_SIZE,  2 * ITEM_COUNT + 1);
        for(int i = 0; i < ITEM_COUNT; i++){
            nk_layout_space_push(ctx, nk_rect(i * ITEM_SIZE,0,ITEM_SIZE,ITEM_SIZE));
            nk_image(ctx, imageHealthEmpty);
            if(health - i >= 1){
                nk_image(ctx, imageHealthFull);
            }else if(health - i > 0.1){
                nk_image(ctx, imageHealthHalf);
            }
            
        }
        nk_layout_space_push(ctx, nk_rect(ITEM_COUNT * ITEM_SIZE,0,ITEM_SIZE,ITEM_SIZE));
        char lvlStr[4];
        sprintf(lvlStr, "%d", (int)expLvl);
        nk_label(ctx, lvlStr, NK_TEXT_CENTERED);

        struct nk_image imageFoodFull = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/food_full.png");
        struct nk_image imageFoodHalf = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/food_half.png");
        struct nk_image imageFoodEmpty = icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/food_empty.png");

        for(int i = ITEM_COUNT - 1; i >= 0; i--){
            nk_layout_space_push(ctx, nk_rect((2 * ITEM_COUNT - i) * ITEM_SIZE,0,ITEM_SIZE,ITEM_SIZE));
            nk_image(ctx, imageFoodEmpty);
            nk_layout_space_push(ctx, nk_rect((2 * ITEM_COUNT - i) * ITEM_SIZE,0,ITEM_SIZE,ITEM_SIZE));
            if(hunger - i >= 1){
                nk_image(ctx, imageFoodFull);
            }else if(hunger - i > 0.1){
                nk_image(ctx, imageFoodHalf);
            }
        }
        nk_layout_space_end(ctx);

        struct nk_image imageExpProgress= icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/experience_bar_progress.png");
        struct nk_image imageExpBakground= icon_load2("./assets/extern_minecraft_assets/assets/minecraft/textures/gui/sprites/hud/experience_bar_background.png");

        nk_layout_space_begin(ctx, NK_STATIC, ITEM_SIZE / 2, INT_MAX);
        nk_layout_space_push(ctx, nk_rect(0,0,ITEM_SIZE * 21,ITEM_SIZE / 2));
        nk_image(ctx, imageExpBakground);
        nk_layout_space_push(ctx, nk_rect(0,0,ITEM_SIZE * 21 * (expLvl - ((int)expLvl)),ITEM_SIZE / 2));
        // nk_layout_row_static(ctx, ITEM_SIZE / 2, 21 *  ITEM_SIZE, 1);
        if (nk_group_begin(ctx, "GroupExp",  NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_static(ctx, ITEM_SIZE / 2, 21 *  ITEM_SIZE, 1);
            nk_image(ctx, imageExpProgress);
            
            nk_group_end(ctx);
        }
        nk_layout_space_end(ctx);
        
        
    }
    nk_end(ctx);
    nk_style_pop_color(ctx);
    nk_style_pop_style_item(ctx);
    
}


CharacterStatusUI::~CharacterStatusUI(){
    
}

