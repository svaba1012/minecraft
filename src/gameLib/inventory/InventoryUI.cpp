#include "gameLib/inventory/InventoryUI.hpp"

using namespace std;

#define INVENTORY_ITEM_SIZE 50
#define PROTECTION_ITEM_SLOTS_NUM 5

#define CRAFTING_ITEM_SLOTS_NUM 4
#define CRAFTING_TABLE_ITEM_SLOTS_NUM 9

// vector<InventoryItem*> protectionItemSlots;
vector<InventoryItem*> craftingItemSlots;
InventoryItem* craftingResultItem;




    

InventoryUI::InventoryUI(Inventory* inv, int numOfCols){
    this->inventory = inv;
    this->numOfCols = numOfCols;
    // protectionItemSlots.resize(PROTECTION_ITEM_SLOTS_NUM);
    // for(int i = 0; i < PROTECTION_ITEM_SLOTS_NUM; i++){
    //     protectionItemSlots[i] = (InventoryItem*) NULL;
    // }
    craftingItemSlots.resize(CRAFTING_TABLE_ITEM_SLOTS_NUM);
    for(int i = 0; i < CRAFTING_TABLE_ITEM_SLOTS_NUM; i++){
        craftingItemSlots[i] = (InventoryItem*) NULL;
    }

}

InventoryItem* InventoryUI::getInventoryItem(int id){
    return this->inventory->itemSlots[id];
}

void InventoryUI::setInventoryItem(InventoryItem* item, int id){
    this->inventory->itemSlots[id] = item;
}

void InventoryUI::setSelectedItem(InventoryItem* item){
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

    unsigned char* imageBufferRGBA = imageBuffer;
    if(bitDepth == 3){
        imageBufferRGBA = new unsigned char[width * height  * 4];
        for(int i = 0; i < width * height; i++){
            imageBufferRGBA[4 * i] = imageBuffer[3 * i];
            imageBufferRGBA[4 * i + 1] = imageBuffer[3 * i + 1];
            imageBufferRGBA[4 * i + 2] = imageBuffer[3 * i + 2];
            imageBufferRGBA[4 * i + 3] = 255;
        }
    }


    GLFWimage image;
    image.height = height;
    image.width = width;
    image.pixels = imageBufferRGBA;
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
    if(n == 4){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
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

void InventoryUI::itemBoxOnMouseLeftClick(InventoryItem** ptrSlotItem){
    InventoryItem* selItem = this->selectedItem;
    InventoryItem* slotItem = *ptrSlotItem;
    
    if(!!selItem && !!slotItem && selItem->getItemTypeId() == slotItem->getItemTypeId()){
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
        this->setSelectedItem(*ptrSlotItem);
        *ptrSlotItem = temp;
        
    }
}

void InventoryUI::itemBoxOnMouseRightClick(InventoryItem** ptrSlotItem ){
    InventoryItem* selItem = this->selectedItem;
    InventoryItem* slotItem = *ptrSlotItem;

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
            *ptrSlotItem = MinecraftInventoryItem::getInstance(selItem);
            selItem->setCount(selItem->getCount() - 1);
            if(selItem->getCount() <= 0){
                delete selItem;
                this->setSelectedItem ((InventoryItem*) NULL);
            }
        }
    }else{
        InventoryItem* slotItem = *ptrSlotItem;
        if(!!slotItem){
            this->setSelectedItem(MinecraftInventoryItem::getInstance(slotItem));
            int half = slotItem->getCount() / 2;
            this->selectedItem->setCount(slotItem->getCount() - half);
            slotItem->setCount(half);
            if(half == 0){
                delete slotItem;
                *ptrSlotItem = (InventoryItem* ) NULL;
            }
        }
    }
}


const string ARMOR_ITEMS_STRING[4] = {"helmet", "chestplate", "leggins", "boots"};
bool isArmorItemType(InventoryItem* item, int i){
    if(!item){
        return true;
    }
    MinecraftInventoryItem* minecraftItem = (MinecraftInventoryItem*) item;
    string itemIdStr = minecraftItem->itemType->getId();
    return itemIdStr.find(ARMOR_ITEMS_STRING[i]) != std::string::npos;
}

ItemType* pattern[3][3] = { {NULL, NULL, NULL}, \
                                {NULL, NULL, NULL}, \
                                {NULL, NULL, NULL}, };

void InventoryUI::searchRecipes(){
    bool isAllNull = true;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            // if(i > 1 || j > 1){
            //     pattern[i][j] = NULL;
            //     continue;
            // }
            InventoryItem* curItem = craftingItemSlots[i * 3 + j];
            if(curItem == NULL){
                pattern[i][j] = NULL;
                continue;
            }
            printf("Item %d. full", i * 3 + j);
            isAllNull = false;
            pattern[i][j] = ((MinecraftInventoryItem*) curItem)->itemType;
        }
    }
    if(craftingResultItem != NULL){
        delete craftingResultItem;
    }
    craftingResultItem = NULL;
    if(isAllNull){
        return;
    }
    printf("Sljaka\n");
    ItemType* resultItemType = Recipe::craftItem(pattern);
    printf("Sljaka\n");
    if(resultItemType != NULL){
        craftingResultItem = new MinecraftInventoryItem(resultItemType);
    }
}

void InventoryUI::consumeIngredients(){
    bool isPatternChanged = false;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(pattern[i][j] != NULL){
                InventoryItem* curItem = craftingItemSlots[i * 3 + j]; 
                int newIngCount = curItem->getCount() - 1;
                if(newIngCount > 0){
                    curItem->setCount(newIngCount);
                    continue;
                }
                isPatternChanged = true;
                delete craftingItemSlots[i * 3 + j];
                craftingItemSlots[i * 3 + j] = NULL;
                pattern[i][j] = NULL;
            }
        }
    }
    if(isPatternChanged){
        searchRecipes();
    }else{
        craftingResultItem = MinecraftInventoryItem::getInstance(selectedItem);
    }
}



void InventoryUI::drawChestInventoryUI(struct nk_context* ctx){

}

void InventoryUI::drawCraftingTableUI(struct nk_context* ctx){

    float rowWidths[4] = {2.5, 3.2, 1, 1};
    for(int i = 0; i < 4; i++){
        rowWidths[i] *= INVENTORY_ITEM_SIZE;
    }

    nk_layout_row(ctx, NK_STATIC, 4.7 * INVENTORY_ITEM_SIZE, 4, rowWidths);
     struct nk_image image;
    struct nk_style_window old_style = ctx->style.window;
    ctx->style.window.group_padding = nk_vec2(0, 0);
    if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
        nk_group_end(ctx);
    }
    if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.3, 1);
        nk_label(ctx, "Crafting", NK_TEXT_LEFT);
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 3);
        for(int i = 0; i < CRAFTING_TABLE_ITEM_SLOTS_NUM; i++){
            InventoryItem* curItem = craftingItemSlots[i];
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                printf("Nesto se zbiva\n");
                this->itemBoxOnMouseLeftClick(&craftingItemSlots[i]);
                searchRecipes();    
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                this->itemBoxOnMouseRightClick(&craftingItemSlots[i]);
                searchRecipes();
            }
            inventory_box(ctx, image, itemCount);
        }

        // ! recepies button
        nk_group_end(ctx);
    }

    if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 1.35, 1);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);
        image = icon_load("./assets/custom_minecraft/right_arrow.png");
        nk_image(ctx, image);
        
        nk_group_end(ctx);
    }

    old_style = ctx->style.window;

    ctx->style.window.group_padding = nk_vec2(0, 0);
    if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 1.35, 1);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 1);

        InventoryItem* curItem = craftingResultItem;
        int itemCount = -1;
        if(curItem != NULL){
            image = icon_load(curItem->getItemIconFilePath());
            itemCount = curItem->getCount();
        }
        if(this->selectedItem == NULL){
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
            // ! IMPLEMENT LATER LOGIC FOR RESULT ITEM
                this->itemBoxOnMouseLeftClick(&craftingResultItem);  
                consumeIngredients();  
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                // ! IMPLEMENT LATER LOGIC FOR RESULT ITEM
                this->itemBoxOnMouseRightClick(&craftingResultItem);
            }
        }
        
        inventory_box(ctx, image, itemCount);
        
        nk_group_end(ctx);
    }
    ctx->style.window = old_style;
}

void InventoryUI::drawFurnaceUI(struct nk_context* ctx){

}

void InventoryUI::drawCharacterInfo(struct nk_context* ctx){
    struct nk_image image;
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
        for(int i = 0; i < PROTECTION_ITEM_SLOTS_NUM - 1; i++){
            InventoryItem* curItem = Character::instance->protectionInventory->itemSlots[i];
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                if(isArmorItemType(this->selectedItem, i)){
                    this->itemBoxOnMouseLeftClick(&Character::instance->protectionInventory->itemSlots[i]);
                }
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                if(isArmorItemType(this->selectedItem, i)){
                    this->itemBoxOnMouseRightClick(&Character::instance->protectionInventory->itemSlots[i]);
                }
            }
            inventory_box(ctx, image, itemCount);
        }
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

        InventoryItem* curItem = Character::instance->protectionInventory->itemSlots[4];
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                // ! TRENUTNO JE HELMET CHANGE TO SHIELD
                if(isArmorItemType(this->selectedItem, 0)){
                    this->itemBoxOnMouseLeftClick(&Character::instance->protectionInventory->itemSlots[4]);
                }
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                // ! TRENUTNO JE HELMET CHANGE TO SHIELD
                if(isArmorItemType(this->selectedItem, 0)){
                    this->itemBoxOnMouseRightClick(&Character::instance->protectionInventory->itemSlots[4]);
                }
            }
        inventory_box(ctx, image, itemCount);
        nk_group_end(ctx);
    }
    // ctx->style.window = old_style;
    if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, INVENTORY_ITEM_SIZE * 0.3, 1);
        nk_label(ctx, "Crafting", NK_TEXT_LEFT);
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 2);
        for(int i = 0; i < CRAFTING_ITEM_SLOTS_NUM; i++){
            InventoryItem* curItem = craftingItemSlots[i];
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                printf("Nesto se zbiva\n");
                this->itemBoxOnMouseLeftClick(&craftingItemSlots[i]);
                searchRecipes();    
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                this->itemBoxOnMouseRightClick(&craftingItemSlots[i]);
                searchRecipes();
            }
            inventory_box(ctx, image, itemCount);
        }

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
        image = icon_load("./assets/custom_minecraft/right_arrow.png");
        nk_image(ctx, image);
        
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

        InventoryItem* curItem = craftingResultItem;
        int itemCount = -1;
        if(curItem != NULL){
            image = icon_load(curItem->getItemIconFilePath());
            itemCount = curItem->getCount();
        }
        if(this->selectedItem == NULL){
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
            // ! IMPLEMENT LATER LOGIC FOR RESULT ITEM
                this->itemBoxOnMouseLeftClick(&craftingResultItem);  
                consumeIngredients();  
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                // ! IMPLEMENT LATER LOGIC FOR RESULT ITEM
                this->itemBoxOnMouseRightClick(&craftingResultItem);
            }
        }
        
        inventory_box(ctx, image, itemCount);
        
        nk_group_end(ctx);
    }
    ctx->style.window = old_style;
}


void InventoryUI::draw(struct nk_context* ctx){
    // const int INVENTORY_ITEM_SIZE = 50;
    struct nk_image image;

    const int xPos = 130, yPos = 10;
    if (nk_begin(ctx, "Inventory", nk_rect(xPos, yPos, 9.9 * INVENTORY_ITEM_SIZE, 9.6 * INVENTORY_ITEM_SIZE), NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
        
        //drawCharacterInfo(ctx);
        drawCraftingTableUI(ctx);
        

        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, this->numOfCols);
        for(int i = 9; i < this->inventory->numOfSlots; i++){
            InventoryItem* curItem = this->getInventoryItem(i);
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                this->itemBoxOnMouseLeftClick(&this->inventory->itemSlots[i]);
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                this->itemBoxOnMouseRightClick(&this->inventory->itemSlots[i]);
            }
            inventory_box(ctx, image, itemCount);
        }
        
        nk_layout_row_dynamic(ctx, 0.2 * INVENTORY_ITEM_SIZE, 1);
        if (nk_group_begin(ctx, "Group", NK_WINDOW_NO_SCROLLBAR)) {
            nk_group_end(ctx);
        }
        nk_layout_row_static(ctx, INVENTORY_ITEM_SIZE, INVENTORY_ITEM_SIZE, 9);
        for(int i = 0; i < 9; i++){
            InventoryItem* curItem = this->getInventoryItem(i);
            int itemCount = -1;
            if(curItem != NULL){
                image = icon_load(curItem->getItemIconFilePath());
                itemCount = curItem->getCount();
            }

            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_LEFT)){
                this->itemBoxOnMouseLeftClick(&this->inventory->itemSlots[i]);
            }
            if(nk_widget_is_mouse_clicked(ctx, NK_BUTTON_RIGHT)){
                this->itemBoxOnMouseRightClick(&this->inventory->itemSlots[i]);
            }

            inventory_box(ctx, image, itemCount);
        }
    }
    nk_end(ctx);
}


InventoryUI::~InventoryUI(){
    
}

