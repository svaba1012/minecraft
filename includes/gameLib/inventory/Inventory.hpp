#pragma once
#include <vector>

#include "gameLib/ui/GameUI2D.hpp"
#include "gameLib/inventory/InventoryItem.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"
#include "Window.hpp"


using namespace std;

class Inventory: public GameUI2D{
private:
    int numOfSlots;
    int numOfCols;
    vector<InventoryItem*> itemSlots;
    
public:
    InventoryItem* selectedItem = (InventoryItem*) NULL;
    Inventory(int initialNumOfSlots, int numOfCols);

    InventoryItem* getInventoryItem(int id);
    void setInventoryItem(InventoryItem* item, int id);
    void setSelectedItem(InventoryItem* item);

    void draw(struct nk_context* ctx);

    ~Inventory();
};