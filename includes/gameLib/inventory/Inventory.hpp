#pragma once
#include <vector>

#include "gameLib/inventory/InventoryItem.hpp"
#include "minecraft/MinecraftInventoryItem.hpp"
#include "Window.hpp"


using namespace std;

class Inventory{
private:
    
public:

    int numOfSlots;
    vector<InventoryItem*> itemSlots;
    InventoryItem* selectedItem = (InventoryItem*) NULL;
    Inventory(int initialNumOfSlots);

    InventoryItem* getInventoryItem(int id);
    void setInventoryItem(InventoryItem* item, int id);
    void setSelectedItem(InventoryItem* item);

    int pushItemInInventory(InventoryItem* item);

    ~Inventory();
};