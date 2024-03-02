#pragma once
#include "gameLib/inventory/InventoryItem.hpp"
#include "minecraft/ItemType.hpp"

class MinecraftInventoryItem: public InventoryItem{
public:
    ItemType* itemType;
    int count;

    MinecraftInventoryItem();
    MinecraftInventoryItem(ItemType* itemType);
    ~MinecraftInventoryItem();
    


    void setCount(int count);
    void setMaximumCount(int maxCount);
    void setItemIconFilePath(char* filePath);

    int getCount();
    int getMaximumCount();
    char* getItemIconFilePath();

    int getItemTypeId();
    // static InventoryItem* getInstance(InventoryItem* inItem);
    static InventoryItem* getInstance(InventoryItem* inItem);
};