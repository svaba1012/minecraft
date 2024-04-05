#pragma once
#include <vector>

#include "gameLib/ui/GameUI2D.hpp"
#include "gameLib/inventory/InventoryItem.hpp"
#include "gameLib/inventory/Inventory.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"
// #include "minecraft/Recipe.hpp"
#include "Window.hpp"



using namespace std;

class Inventory;

class InventoryUI: public GameUI2D{
private:
    Inventory* inventory;
    int numOfCols;
    
    
public:
    InventoryItem* selectedItem = (InventoryItem*) NULL;
    
    InventoryUI(Inventory* inv, int numOfCols);

    InventoryItem* getInventoryItem(int id);
    void setInventoryItem(InventoryItem* item, int id);
    void setSelectedItem(InventoryItem* item);

    void itemBoxOnMouseLeftClick(InventoryItem** ptrSlotItem );
    void itemBoxOnMouseRightClick(InventoryItem** ptrSlotItem );

    void searchRecipes();
    void consumeIngredients();


    void drawCharacterInfo(struct nk_context* ctx);
    void drawChestInventoryUI(struct nk_context* ctx);
    void drawCraftingTableUI(struct nk_context* ctx);
    void drawFurnaceUI(struct nk_context* ctx);

    void draw(struct nk_context* ctx);

    ~InventoryUI();
};