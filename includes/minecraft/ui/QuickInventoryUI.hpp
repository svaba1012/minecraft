#pragma once
#include <vector>

#include "gameLib/ui/GameUI2D.hpp"
#include "gameLib/inventory/InventoryItem.hpp"
#include "gameLib/inventory/Inventory.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"
#include "Window.hpp"


using namespace std;

class Inventory;

class QuickInventoryUI: public GameUI2D{
private:
    Inventory* inventory;
    int numOfItems;
    
    
public:
    
    QuickInventoryUI(Inventory* inv, int numOfItems);

    InventoryItem* getInventoryItem(int id);
    // void setInventoryItem(InventoryItem* item, int id);
    // void setSelectedItem(InventoryItem* item);

    // void itemBoxOnMouseLeftClick(InventoryItem** ptrSlotItem );
    // void itemBoxOnMouseRightClick(InventoryItem** ptrSlotItem );

    void draw(struct nk_context* ctx);

    ~QuickInventoryUI();
};