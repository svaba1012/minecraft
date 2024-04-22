#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include "graphics/Texture.hpp"
#include "game/Character.hpp"


#define TEXTURE_NUM_PER_BLOCK 3

using json = nlohmann::json;
using namespace std;

#define BLOCK_STATE_ITEM_CONTENT 0x01
#define BLOCK_STATE_FURNACE_STATE 0x02

//!FOR FUTURE USAGE
#define BLOCK_STATE1 0x04
#define BLOCK_STATE2 0x08
#define BLOCK_STATE3 0x10
#define BLOCK_STATE4 0x20
#define BLOCK_STATE5 0x40
#define BLOCK_STATE6 0x80

// class Character;
class Block;

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
    int id;

    int stateType = 0;
    
    float hardness;

    bool needsStoneTool;
    bool needsIronTool;
    bool needsDiamondTool;

    bool mineableWithAxe;
    bool mineableWithPickaxe;
    bool mineableWithHoe;
    bool mineableWithShovel;

    void (*accessBlock)(Block* block);


    static void init();
    static BlockType* getTypeByName(string name);
    static BlockType* getTypeByVariantName(string name);
    
    void loadData();
    Texture* getTexturesByVariantName(string variantName, int side);
    ~BlockType();
};


