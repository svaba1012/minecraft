#include "minecraft/MinecraftInventoryItem.hpp"

MinecraftInventoryItem::MinecraftInventoryItem(ItemType* itemType): InventoryItem() {
    this->itemType = itemType;
    this->count = 1;
}

MinecraftInventoryItem::MinecraftInventoryItem(): InventoryItem() {
    this->count = 1;
}

MinecraftInventoryItem::~MinecraftInventoryItem(){

}

void MinecraftInventoryItem::setCount(int count){
    if(count >= 0 && count <= this->getMaximumCount()){
        this->count = count;
    }
}
void MinecraftInventoryItem::setMaximumCount(int maxCount){
    return;
}
    
void MinecraftInventoryItem::setItemIconFilePath(char* filePath){
    return;
}

int MinecraftInventoryItem::getCount(){
    return this->count;
}

int MinecraftInventoryItem::getMaximumCount(){
    return itemType->stackability;
}

char*  MinecraftInventoryItem::getItemIconFilePath(){
    char* str = itemType->iconFilePath; 
    return str;
}

int MinecraftInventoryItem::getItemTypeId(){
    return itemType->getTypeId();
}

InventoryItem* MinecraftInventoryItem::getInstance(InventoryItem* inItem){
    MinecraftInventoryItem* newItem = new MinecraftInventoryItem();
    newItem->itemType = ((MinecraftInventoryItem*)inItem)->itemType;
    newItem->count = 1;
    return newItem;
}