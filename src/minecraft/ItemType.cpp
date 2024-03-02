#include "minecraft/ItemType.hpp"
#include <iostream>

json ItemType::itemsData;
int ItemType::typeNum;

ItemType* ItemType::types;
map<string, int> ItemType::nameMap;
map<string, int> ItemType::idMap;



void ItemType::init(){
    std::ifstream f("./assets/custom_minecraft/itemlist.json");
    itemsData = json::parse(f);

    ItemType::typeNum = itemsData.size();
    ItemType::types = new ItemType[typeNum];
    for(int i = 0; i < ItemType::typeNum; i++){
        ItemType::types[i].itemData = itemsData.at(i);
        ItemType::types[i].typeId = i;
        
        ItemType::nameMap.insert({(string) itemsData.at(i).at("item"), i});
        ItemType::idMap.insert({(string) itemsData.at(i).at("id"), i});
    }
}

int ItemType::getTypeId(){
    return this->typeId;
}

ItemType* ItemType::getTypeByName(string name){
    int index = ItemType::nameMap[name]; 
    ItemType* itemType = &ItemType::types[index];
    itemType->loadData();
    // printf("Proba JSON aloo %d\n", index);
    // printf("Proba JSON alo %f\n", (float)BlockType::types[index].blockData.at("hardness"));
    return itemType;
}

ItemType* ItemType::getTypeById(string name){
    int index = ItemType::idMap[name]; 
    ItemType* itemType = &ItemType::types[index];
    itemType->loadData();
    // printf("Proba JSON aloo %d\n", index);
    // printf("Proba JSON alo %f\n", (float)BlockType::types[index].blockData.at("hardness"));
    return itemType;
}


// ! CHANGE
Texture* ItemType::makeTexture(string texturePath){
// primer putanje do teksture
// "minecraft:block/acacia_planks"
// odseci minecraft:
// nalepi prefiks // PATH ./assets/minecraft_assets/minecraft/textures/
        
    string subStr = "minecraft:";
    size_t pos = texturePath.find(subStr);
    if (pos != std::string::npos){
        // Remove found substring from string
        texturePath.erase(pos, subStr.length());
    }
    string fullTexturePath ="./assets/extern_minecraft_assets/assets/minecraft/textures/" + texturePath + ".png";
      char* fullTexturePathChar = new char[fullTexturePath.length() + 1];
      strcpy(fullTexturePathChar, fullTexturePath.c_str()); 
      this->iconFilePath = fullTexturePathChar;
    return new Texture(fullTexturePathChar);
}



void ItemType::loadData(){
    if(this->isLoaded){
        return;
    }
    this->isLoaded = true;

    this->id = (string) this->itemData.at("id");
    json tempJson = this->itemData.at("stackability");
    if( tempJson.is_string() && ((string) tempJson).compare("Unstackable") == 0){
        this->stackability = 1;
    }else{
        this->stackability = (int) tempJson;
    }

    this->survival_obtainable = (bool) (((string) this->itemData.at("survival_obtainable")).compare("Yes") == 0);
    this->is_weapon = (bool) (((string)this->itemData.at("is_weapon")).compare("Yes") == 0);
    this->peaceful_obtainable = (bool) (((string)this->itemData.at("peaceful_obtainable")).compare("Yes") == 0);
    this->oneninteen = (bool) (((string)this->itemData.at("oneninteen")).compare("1.19+") == 0);
    this->renewable = (bool) (((string)this->itemData.at("renewable")).compare("Yes") == 0);

    // !PROCESS
    tempJson = this->itemData.at("hunger");
    this->hunger = 0.0;
    if(tempJson.is_string() && ((string)tempJson).compare("None") != 0){
        this->hunger = (float) tempJson;
    }
    tempJson = this->itemData.at("saturation");
    this->saturation = 0.0;
    if(tempJson.is_string() && ((string)tempJson).compare("None") != 0){
        this->saturation = (float) tempJson;
    }
    tempJson = this->itemData.at("xp_from_smelting");
    this->xp_from_smelting = 0.0;
    if(tempJson.is_string() && ((string)tempJson).compare("Non-Smeltable") != 0){
        this->xp_from_smelting = (float) tempJson;
    }
    tempJson = this->itemData.at("fuel_duration");
    this->fuel_duration = 0.0;
    if(tempJson.is_string() && ((string)tempJson).compare("None") != 0){
        this->fuel_duration = (float) tempJson;
    }
    tempJson = this->itemData.at("cooldown");
    this->cooldown = 0.0;
    if(tempJson.is_string() && ((string)tempJson).compare("No") != 0){
        this->cooldown = (float) tempJson;
    }

    /*
    ///?
    Clears Poison
    None
    Inedible
    3 hungera, jos dva fale
    Hunger for 30 seconds (30% chance)


    Poison for 5 seconds
    Poison for 5 seconds (60% chance)
    Regeneration II for 5 seconds
    Regeneration II for 20 seconds
    The following for 3-11 seconds, depending on the flower used:<br>Regeneration<br>Jump Boost<br>Poison<br>Wither<br>Weakness<br>Blindness<br>Fire Resistance<br>Saturation<br>Night Vision
    The player may be teleported randomly.
    */
    // ! PROCCESS
    // ? this->effect_when_eaten = () this->itemData.at("effect_when_eaten");
    


    // * Possible values  
    // No 
    // With Furnace Only
    // With Blast Furnace
    // With Smoker
    string smeltableStr = (string) this->itemData.at("smeltable");
    if(smeltableStr.compare("No")){
        this->smeltable = NO;
    }else if(smeltableStr.compare("With Furnace Only")){
        this->smeltable = SMELTABLE_WITH_FURNACE;
    }else if(smeltableStr.compare("With Blast Furnace")){
        this->smeltable = SMELTABLE_WITH_BLAST_FURNACE;
    }else if(smeltableStr.compare("With Smoker")){
        this->smeltable = SMELTABLE_WITH_SMOKER;
    }
    smeltableStr = (string) this->itemData.at("obtainable_by_smelting");
    if(smeltableStr.compare("No")){
        this->obtainable_by_smelting = NO;
    }else if(smeltableStr.compare("With Furnace Only")){
        this->obtainable_by_smelting = SMELTABLE_WITH_FURNACE;
    }else if(smeltableStr.compare("With Blast Furnace")){
        this->obtainable_by_smelting = SMELTABLE_WITH_BLAST_FURNACE;
    }else if(smeltableStr.compare("With Smoker")){
        this->obtainable_by_smelting = SMELTABLE_WITH_SMOKER;
    }

    this->attack_speed = (float) this->itemData.at("attack_speed");
    this->attack_damage = (float) this->itemData.at("attack_damage");
    this->damage_per_second = (float) this->itemData.at("damage_per_second");


    tempJson = this->itemData.at("breaking_speed");
    if(tempJson.is_number() && !tempJson.contains("By default")){
        this->breaking_speed = (float) tempJson; 
    }else{
        this->breaking_speed = (float) (tempJson.at("By default"));
    }
    

    this->textureList = new Texture*[TEXTURE_NUM_PER_BLOCK];
    
        
        
        // convert variant name to lowercase with underscore and good path
        // PATH ./assets/minecraft_assets/minecraft/models/block + name

        std::ifstream f("./assets/extern_minecraft_assets/assets/minecraft/models/item/" + this->id + ".json");
        json itemModel; 
        this->isBlock = f.fail();
        if(!this->isBlock){
            itemModel = json::parse(f);
            if(!itemModel.contains("textures")){
                this->isBlock = true;
            }else{
                json itemTexturesJSON = itemModel.at("textures");
                Texture* itemTexture;
                if(itemTexturesJSON.contains("layer0")){
                    itemTexture = makeTexture((string)itemTexturesJSON.at("layer0"));
                    itemTexture->loadTexture();
                    this->textureList[0] = itemTexture;
                }
            }
            
        }
        if (this->isBlock){
            std::ifstream f1("./assets/extern_minecraft_assets/assets/minecraft/models/block/" + this->id + ".json");
            itemModel = json::parse(f1);
            this->isBlock = true;
            json itemTexturesJSON = itemModel.at("textures");
            Texture* itemTexture;
            if(itemTexturesJSON.contains("all")){

            // ! IMPLEMENT ICON BUILDING
            itemTexture = makeTexture((string)itemTexturesJSON.at("all"));
            itemTexture->loadTexture();
            this->textureList[0] = itemTexture;
            this->textureList[1] = itemTexture;
            this->textureList[2] = itemTexture;
        }else if(itemTexturesJSON.contains("end") && itemTexturesJSON.contains("side")){
            itemTexture = makeTexture((string)itemTexturesJSON.at("end"));
            itemTexture->loadTexture();
            this->textureList[0] = itemTexture;
            this->textureList[2] = itemTexture;
            itemTexture = makeTexture((string)itemTexturesJSON.at("side"));
            itemTexture->loadTexture();
            this->textureList[1] = itemTexture;
        }else if(itemTexturesJSON.contains("top") && itemTexturesJSON.contains("side") && itemTexturesJSON.contains("bottom")){
            itemTexture = makeTexture((string)itemTexturesJSON.at("top"));
            itemTexture->loadTexture();
            this->textureList[0] = itemTexture;
            itemTexture = makeTexture((string)itemTexturesJSON.at("side"));
            itemTexture->loadTexture();
            this->textureList[1] = itemTexture;
            itemTexture = makeTexture((string)itemTexturesJSON.at("bottom"));
            itemTexture->loadTexture();
            this->textureList[2] = itemTexture;
        }    
            
        }
        

        return;




        // ? what if item is block
        // ako ne otvara onda citaj iz bloka 
        // if(false){
        //     std::ifstream f("./assets/extern_minecraft_assets/assets/minecraft/models/block/" + this->id + ".json");
        //     itemModel = json::parse(f);
        //     this->isBlock = true;
        // }
    

        // json itemTexturesJSON = itemModel.at("textures");
        // Texture* itemTexture;
        // if(itemTexturesJSON.contains("all")){

        //     itemTexture = makeTexture((string)itemTexturesJSON.at("all"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK] = itemTexture;
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = itemTexture;
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = itemTexture;
        // }else if(itemTexturesJSON.contains("end") && itemTexturesJSON.contains("side")){
        //     itemTexture = makeTexture((string)itemTexturesJSON.at("end"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK] = itemTexture;
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = itemTexture;
        //     itemTexture = makeTexture((string)itemTexturesJSON.at("side"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = itemTexture;
        // }else if(itemTexturesJSON.contains("top") && itemTexturesJSON.contains("side") && itemTexturesJSON.contains("bottom")){
        //     itemTexture = makeTexture((string)itemTexturesJSON.at("top"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK] = itemTexture;
        //     itemTexture = makeTexture((string)itemTexturesJSON.at("side"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = itemTexture;
        //     itemTexture = makeTexture((string)itemTexturesJSON.at("bottom"));
        //     itemTexture->loadTexture();
        //     this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = itemTexture;
        // }    
}

Texture* ItemType::getTextures(int side){
    return this->textureList[side];
}

ItemType::ItemType(){
    
}

ItemType::~ItemType(){

}