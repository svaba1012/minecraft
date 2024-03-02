#pragma once


class InventoryItem{
public:
    virtual void setCount(int count) = 0;
    virtual void setMaximumCount(int maxCount) = 0;
    virtual void setItemIconFilePath(char* filePath) = 0;

    virtual int getCount() = 0;
    virtual int getMaximumCount() = 0;

    int getSpaceLeft();
    virtual char* getItemIconFilePath() = 0;

    virtual int getItemTypeId() = 0;
    

    
};