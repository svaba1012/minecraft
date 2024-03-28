#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include "graphics/Texture.hpp"

#define TEXTURE_NUM_PER_BLOCK 3

using json = nlohmann::json;
using namespace std;

enum SMELTABLE{
    NO, SMELTABLE_WITH_FURNACE, SMELTABLE_WITH_BLAST_FURNACE, SMELTABLE_WITH_SMOKER
};

class ItemType{
private:
    static json itemsData;
    static int typeNum;
    static ItemType* types;
    static map<string, int> nameMap;
    static map<string, int> idMap;
    Texture** textureList;
    ItemType();
    // string id;
    
public:
    int typeId;
    string name;
    json itemData;
    bool isLoaded = false;
    map<string, int> variantMap;
    string* variants;
    int variantNum;
    bool isBlock;
    char* iconFilePath;


    // item features
    string id;
    int stackability; 
    bool survival_obtainable; 
    bool peaceful_obtainable;
    bool is_weapon; 
    bool oneninteen; 
    bool renewable;
    int effect_when_eaten; 
    int obtainable_by_smelting; 
    int smeltable; 
    float hunger;
    float saturation; 
    float attack_speed; 
    float attack_damage; 
    float damage_per_second; 
    float breaking_speed; 
    float cooldown;
    float xp_from_smelting; 
    float fuel_duration; 

    


    static void init();
    static ItemType* getTypeByName(string name);
    static ItemType* getTypeById(string id);
    int getTypeId();
    string getId();
    string getName();
    
    void loadData();
    Texture* makeTexture(string texturePath);
    Texture* getTextures(int side);
    ~ItemType();
};
