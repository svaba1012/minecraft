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
    static map<string, int> variantNameMap;
    Texture** textureList;
    BlockType(/* args */);

    bool readJsonBool(char* fieldName);
    
public:
    json blockData;
    bool isLoaded = false;
    map<string, int> variantMap;
    string* variants;
    int variantNum;

    float hardness;

    bool needsStoneTool;
    bool needsIronTool;
    bool needsDiamondTool;

    bool mineableWithAxe;
    bool mineableWithPickaxe;
    bool mineableWithHoe;
    bool mineableWithShovel;



    static void init();
    static BlockType* getTypeByName(string name);
    static BlockType* getTypeByVariantName(string name);
    
    void loadData();
    Texture* getTexturesByVariantName(string variantName, int side);
    ~BlockType();
};


