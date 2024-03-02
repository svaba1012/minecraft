#include "gameLib/inventory/Inventory.hpp"

using namespace std;

#define INVENTORY_ITEM_SIZE 50

Inventory::Inventory(int initialNumOfSlots, int numOfCols){
    this->numOfSlots = initialNumOfSlots;
    this->numOfCols = numOfCols;
    this->itemSlots.resize(initialNumOfSlots);
    for(int i = 0; i < initialNumOfSlots; i++){
        this->itemSlots[i] = (InventoryItem*) NULL;
    }
}

InventoryItem* Inventory::getInventoryItem(int id){
    return this->itemSlots[id];
}

void Inventory::setInventoryItem(InventoryItem* item, int id){
    this->itemSlots[id] = item;
}

void Inventory::setSelectedItem(InventoryItem* item){
    this->selectedItem = item;
    if(item==NULL){
        GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        glfwSetCursor(Window::window, cursor);
        return;
    }

    int width, height, bitDepth;
    unsigned char* imageBuffer = stbi_load(item->getItemIconFilePath(), &width, &height, &bitDepth, 0);

    // assume is divisible by 4
    if(width < INVENTORY_ITEM_SIZE || height < INVENTORY_ITEM_SIZE){
        int widthScale = INVENTORY_ITEM_SIZE / width;
        int heightScale = INVENTORY_ITEM_SIZE / height;

        int scale = widthScale;
        if(scale < heightScale){
            scale = heightScale;
        }

        cout << scale << endl;

        unsigned char* resizedImage = new unsigned char[width * height * scale * scale * bitDepth];

        stbir_resize_uint8(imageBuffer, width, height, 0, resizedImage, width * scale, height * scale, 0, bitDepth);

        imageBuffer = resizedImage;
        width *= scale;
        height *= scale;
    }


    GLFWimage image;
    image.height = height;
    image.width = width;
    image.pixels = imageBuffer;
    GLFWcursor* cursor = glfwCreateCursor(&image, INVENTORY_ITEM_SIZE / 2, INVENTORY_ITEM_SIZE / 2);


    if (cursor == NULL){
        // throw new RuntimeException("Error creating cursor");
    }
    
    // Set the cursor on a window
    glfwSetCursor(Window::window, cursor);
}

struct nk_image icon_load(const char *filename){
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


void inventory_box( struct nk_context *ctx, struct nk_image image, int count){
    // struct nk_context *ctx = &context;
    // const int INVENTORY_ITEM_SIZE = 50;
    struct nk_style_window old_style = ctx->style.window;
    ctx->style.window.group_padding = nk_vec2(0,0);
    float rowWidths[2] = {40, 20};
    if (nk_group_begin(ctx, "Group", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        // nk_layout_row_dynamic(ctx, 50, 2);
        // nk_layout_row_static(ctx, 50, 50, 2);
        nk_layout_space_begin(ctx, NK_STATIC, 50, INT_MAX);
        nk_layout_space_push(ctx, nk_rect(0,0,50,50));

        // nk_layout_row(ctx, NK_STATIC, INVENTORY_ITEM_SIZE, 2, rowWidths);
        if (nk_group_begin(ctx, "GroupImage",  NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_static(ctx, 50, 50, 1);
            
            if(count > -1){
                nk_image(ctx, image);
            }
            nk_group_end(ctx);
        }
        if(count > 1){
            nk_layout_space_push(ctx, nk_rect(30,30,15,15));
            if (nk_group_begin(ctx, "GroupCount",  NK_WINDOW_NO_SCROLLBAR)) {
                nk_layout_row_static(ctx, 15, 15, 1);
                
                    char countStr[10];
                    sprintf(countStr, "%d", count);
                    nk_label(ctx, countStr, NK_TEXT_RIGHT);
                nk_group_end(ctx);
            }
        }
        nk_layout_space_end(ctx);
        ctx->style.window.group_padding = nk_vec2(0,0);
        // nk_image(ctx, image);
        
        nk_group_end(ctx);
    }
    // ctx->style.window = old_style;
}

void Inventory::draw(struct nk_context* ctx){
    // const int INVENTORY_ITEM_SIZE = 50;
    const int xPos = 130, yPos = 10;
    if (nk_begin(ctx, "Inventory", nk_rect(xPos, yPos, 9.9 * INVENTORY_ITEM_SIZE, 9.6 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
        struct nk_image image = icon_load("./assets/extern_minecraft_assets/assets/minecraft/textures/item/diamond_sword.png");
        float rowWidths[6] = {1, 3, 1, 2.2, 1, 1};
        for(int i = 0; i < 6; i++){
            rowWidths[i] *= INVENTORY_ITEM_SIZE;
        }

        nk_layout_row(ctx, NK_STATIC, 4.7 * INVENTORY_ITEM_SIZE, 6, rowWidths);
        struct nk_style_window old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            // nk_layout_row_dynamic(ctx, 50, 1);
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);
            nk_group_end(ctx);
        }
        // ctx->style.window = old_style;
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }

        // old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            inventory_box(ctx, image, 10);
            nk_group_end(ctx);
        }
        // ctx->style.window = old_style;
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.3, 1);
            nk_label(ctx, "Crafting", NK_TEXT_LEFT);
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 2);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);
            inventory_box(ctx, image, 10);

            // ! recepies button
            nk_group_end(ctx);
        }

        // old_style = ctx->style.window;
        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image, 10);
            
            nk_group_end(ctx);
        }
        // ctx->style.window = old_style;
        old_style = ctx->style.window;

        ctx->style.window.group_padding = nk_vec2(0, 0);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.95, 1);
            if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
                nk_group_end(ctx);
            }
            nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
            inventory_box(ctx, image, 10);
            
            nk_group_end(ctx);
        }
        ctx->style.window = old_style;

        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, this->numOfCols);
        for(int i = 0; i < numOfSlots; i++){
            InventoryItem* curItem = this->getInventoryItem(i);
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                InventoryItem* selItem = this->selectedItem;
                InventoryItem* slotItem = this->itemSlots[i];
                if(!!selItem && !!slotItem && selItem->getItemTypeId() == selItem->getItemTypeId()){
                    int diff = slotItem->getSpaceLeft();
                    if(diff < selItem->getCount()){
                        slotItem->setCount(slotItem->getCount() + diff);
                        selItem->setCount(selItem->getCount() - diff);
                    }else{
                        slotItem->setCount(slotItem->getCount() + selItem->getCount());
                        delete selItem;
                        this->setSelectedItem((InventoryItem*) NULL);
                    }

                }else{
                    InventoryItem* temp = this->selectedItem;
                    this->setSelectedItem(this->itemSlots[i]);
                    this->itemSlots[i] = temp;
                }
            
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                InventoryItem* selItem = this->selectedItem;
                InventoryItem* slotItem = this->itemSlots[i];

                if(selItem != NULL){
                    if(slotItem != NULL){
                        if(selItem->getItemTypeId() == slotItem->getItemTypeId()){
                            if(slotItem->getSpaceLeft()){
                                slotItem->setCount(slotItem->getCount() + 1);
                                selItem->setCount(selItem->getCount() - 1);
                                printf("%d\n", selItem->getCount());
                                if(selItem->getCount() <= 0){
                                    
                                    delete selItem;
                                    this->setSelectedItem ((InventoryItem*) NULL);
                                }
                            }
                        }
                    }else{
                        this->itemSlots[i] = MinecraftInventoryItem::getInstance(selItem);
                        selItem->setCount(selItem->getCount() - 1);
                        if(selItem->getCount() <= 0){
                            delete selItem;
                            this->setSelectedItem ((InventoryItem*) NULL);
                        }
                    }
                }else{
                    InventoryItem* slotItem = this->itemSlots[i];
                    if(!!slotItem){
                        this->setSelectedItem(MinecraftInventoryItem::getInstance(slotItem));
                        int half = slotItem->getCount() / 2;
                        this->selectedItem->setCount(slotItem->getCount() - half);
                        slotItem->setCount(half);
                        if(half == 0){
                            delete slotItem;
                            this->itemSlots[i] = (InventoryItem* ) NULL;
                        }
                    }
                }
                
            }

            inventory_box(ctx, image, itemCount);
        }
        
        nk_layout_row_dynamic(ctx, 0.2 * INVENTORY_ITEM_SIZE, 1);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
        for(int j = 0; j < 9; j++){
            inventory_box(ctx, image, 10);
        }

        // inventory_box(ctx, image, 10);
    }
    nk_end(ctx);
}


Inventory::~Inventory(){
    for(int i = 0; i < this->itemSlots.size(); i++){
        delete this->itemSlots[i]; 
    }
}

