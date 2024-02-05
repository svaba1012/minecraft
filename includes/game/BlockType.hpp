#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include "graphics/Texture.hpp"

#define TEXTURE_NUM_PER_BLOCK 3

using json = nlohmann::json;
using namespace std;

class BlockType{
private:
    /* data */
    static json blocksData;
    static int typeNum;
    static BlockType* types;
    static map<string, int> nameMap;
    Texture** textureList;
    BlockType(/* args */);
    
public:
    json blockData;
    bool isLoaded = false;
    map<string, int> variantMap;
    string* variants;
    int variantNum;

    static void init();
    static BlockType* getTypeByName(string name);
    
    void loadData();
    Texture* getTexturesByVariantName(string variantName, int side);
    ~BlockType();
};


