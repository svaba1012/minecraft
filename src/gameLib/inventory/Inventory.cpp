#include "gameLib/inventory/InventoryUI.hpp"

using namespace std;

#define INVENTORY_ITEM_SIZE 50
#define PROTECTION_ITEM_SLOTS_NUM 5

#define CRAFTING_ITEM_SLOTS_NUM 4

Inventory::Inventory(int initialNumOfSlots){
    this->numOfSlots = initialNumOfSlots;
    this->itemSlots.resize(initialNumOfSlots);
    for(int i = 0; i < initialNumOfSlots; i++){
        this->itemSlots[i] = (InventoryItem*) NULL;
    }
}


//returns place inserted at, otherwise -1
int Inventory::pushItemInInventory(InventoryItem* item){
    for(int i = 0; i < numOfSlots; i++){
        if(itemSlots[i] != NULL && itemSlots[i]->getItemTypeId() == item->getItemTypeId()){
            if(itemSlots[i]->getSpaceLeft() > 0){
                itemSlots[i]->setCount(itemSlots[i]->getCount() + 1);
                return i;
            }
        }
    }
    for(int i = 0; i < numOfSlots; i++){
        if(itemSlots[i] == NULL){
            itemSlots[i] = item;
            return i;
        }
    }
    return -1;
}

InventoryItem* Inventory::getInventoryItem(int id){
    return this->itemSlots[id];
}

void Inventory::setInventoryItem(InventoryItem* item, int id){
    this->itemSlots[id] = item;
}

Inventory::~Inventory(){
    for(int i = 0; i < this->itemSlots.size(); i++){
        delete this->itemSlots[i]; 
    }
}

