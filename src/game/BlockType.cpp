#include "game/BlockType.hpp"
#include <iostream>

json BlockType::blocksData;
int BlockType::typeNum;

BlockType* BlockType::types;
map<string, int> BlockType::nameMap;

void BlockType::init(){
    std::ifstream f("./assets/custom_minecraft/blocklist.json");
    blocksData = json::parse(f);

    BlockType::typeNum = blocksData.size();
    BlockType::types = new BlockType[typeNum];
    for(int i = 0; i < BlockType::typeNum; i++){
        BlockType::types[i].blockData = blocksData.at(i);
        BlockType::nameMap.insert({(string) blocksData.at(i).at("block"), i});
    }
}

BlockType* BlockType::getTypeByName(string name){
    int index = BlockType::nameMap[name]; 
    // printf("Proba JSON aloo %d\n", index);
    // printf("Proba JSON alo %f\n", (float)BlockType::types[index].blockData.at("hardness"));
    return &BlockType::types[index];
}

Texture* makeTexture(string texturePath){
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
    string fullTexturePath ="./assets/minecraft_assets/minecraft/textures/" + texturePath + ".png";
      char* fullTexturePathChar = new char[fullTexturePath.length() + 1];
      strcpy(fullTexturePathChar, fullTexturePath.c_str()); 
    return new Texture(fullTexturePathChar);
}

void BlockType::loadData(){
    if(this->isLoaded){
        return;
    }
    this->isLoaded = true;
    this->variantNum = this->blockData.at("variants").size();
    this->variants = new string[this->variantNum];

    if(variantNum == 1){
        this->variants[0] = (string) this->blockData.at("variants");
    }else{
        for(int i = 0; i < this->variantNum; i++){
            this->variants[i] = (string) this->blockData.at("variants").at(i);
        }
    }

    for(int i = 0; i < this->variantNum; i++){
        this->variantMap.insert({(string) this->variants[i], i});
    }

    this->textureList = new Texture*[variantNum * TEXTURE_NUM_PER_BLOCK];
    for(int i = 0; i < this->variantNum; i++){
        string curVariant = variants[i];
        
        
        // convert variant name to lowercase with underscore and good path
        // PATH ./assets/minecraft_assets/minecraft/models/block + name

        for(int i = 0; i < curVariant.length(); i++){
            if(curVariant[i] == ' '){
                curVariant[i] = '_';
            }
            if(curVariant[i] >= 'A' && curVariant[i] <= 'Z'){
                curVariant[i] += 'a' - 'A';
            }
        }

        std::cout << "Varijanta je " << curVariant << " a niz je " << variants[i] << std::endl; 

        std::ifstream f("./assets/minecraft_assets/minecraft/models/block/" + curVariant + ".json");
        json blockModel = json::parse(f);

        json blockTexturesJSON = blockModel.at("textures");
        Texture* blockTexture;
        if(blockTexturesJSON.contains("all")){

            blockTexture = makeTexture((string)blockTexturesJSON.at("all"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK] = blockTexture;
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = blockTexture;
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = blockTexture;
        }else if(blockTexturesJSON.contains("end") && blockTexturesJSON.contains("side")){
            blockTexture = makeTexture((string)blockTexturesJSON.at("end"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK] = blockTexture;
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = blockTexture;
            blockTexture = makeTexture((string)blockTexturesJSON.at("side"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = blockTexture;
        }else if(blockTexturesJSON.contains("top") && blockTexturesJSON.contains("side") && blockTexturesJSON.contains("bottom")){
            blockTexture = makeTexture((string)blockTexturesJSON.at("top"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK] = blockTexture;
            blockTexture = makeTexture((string)blockTexturesJSON.at("side"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 1] = blockTexture;
            blockTexture = makeTexture((string)blockTexturesJSON.at("bottom"));
            blockTexture->loadTexture();
            this->textureList[i * TEXTURE_NUM_PER_BLOCK + 2] = blockTexture;
        }

         

            

            
    }
}

Texture* BlockType::getTexturesByVariantName(string variantName, int side){
    int index = this->variantMap[variantName];
    return this->textureList[index * TEXTURE_NUM_PER_BLOCK + side];
}

BlockType::BlockType(/* args */){
    
}

BlockType::~BlockType()
{

}