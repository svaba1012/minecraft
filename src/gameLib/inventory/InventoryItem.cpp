#include "gameLib/inventory/InventoryItem.hpp"


int InventoryItem::getSpaceLeft(){
    return  this->getMaximumCount() - this->getCount() ;
}