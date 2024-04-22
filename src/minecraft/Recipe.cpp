#include "minecraft/Recipe.hpp"

RecipeBranch* Recipe::rootRecipeBranch;
RecipeBranch* Recipe::rootRecipeBranchShapeless;
map<ItemType*, ItemType*> Recipe::smeltRecipeMap;


Recipe::Recipe(){

}
Recipe::~Recipe(){
    
}

void Recipe::loadRecipes(){
    rootRecipeBranch = new RecipeBranch();
    rootRecipeBranchShapeless = new RecipeBranch();
    std::ifstream f("./assets/extern_minecraft_assets/data/minecraft/recipes/_list.json");
    json jsonFileList = json::parse(f);
    json jsonList = jsonFileList.at("files");

    for(int i = 0; i < jsonList.size(); i++){
        Recipe* curRecipe = new Recipe();
        string filename = "./assets/extern_minecraft_assets/data/minecraft/recipes/";
        filename = filename + (string) jsonList.at(i);
        curRecipe->init(filename);
    }

} 

void Recipe::init(string filename){
    std::ifstream f(filename);
    json jsonRecipe = json::parse(f);
    
    string typeStr = (string) jsonRecipe.at("type");
   
    cout << typeStr << endl;
    if(typeStr.find("crafting_shaped") != std::string::npos){
        // printf("Type is crafting...\n");
        // cout << "Initialising recipe for " <<  filename << endl;
        type = CRAFT_RECIPE_TYPE_SHAPED;
        json itemKeys = jsonRecipe.at("key");
        json patternJson = jsonRecipe.at("pattern");
        int patternRowsNum = patternJson.size();
        int patternColsNum = 0;
        map<char, ItemType*> ingredientsChar;
        // printf("Init patterns...\n");
        RecipeBranch* curRecipeBranch = rootRecipeBranch;
        for(int i = 0; i < 3; i++){
            // printf("Iterating %d. row...\n", i + 1);
            if(i >= patternRowsNum){
                break;
            }
            string currentRow = (string) patternJson.at(i);
            patternColsNum = currentRow.length();
            for(int j = 0; j < 3; j++){
                // printf("Iterating %d. column...\n", j + 1);
                if(j >= patternColsNum){
                    break;
                }
                patternStr[i][j] = currentRow[j];
                if(currentRow[j] == ' '){
                    continue;
                }
                if(ingredientsChar.find(currentRow[j]) == ingredientsChar.end()){
                    // printf("New ingredient found in pattern...\n");

                    char tempStr[2];
                    tempStr[0] = currentRow[j];
                    tempStr[1] = '\0';
                    if(!itemKeys.at(tempStr).contains("item")){
                        json tempTag = itemKeys.at(tempStr); 
                        if(tempTag.contains("tag") && ((string)tempTag.at("tag")).compare("minecraft:planks") != 0){
                            // printf("Unsupported recipe with tags... Fix later\n");
                            // cout << "Tag: " << (string)itemKeys.at(tempStr).at("tag") << endl;
                        }
                        return;
                    }
                    string ingredientTypeStr = (string)itemKeys.at(tempStr).at("item");
                    ingredientTypeStr = ingredientTypeStr.substr(10, ingredientTypeStr.length());
                    // cout << "INGREDIENT TYPE: " << ingredientTypeStr << " IN char: " <<currentRow[j] << endl;
                    // printf("Temp text\n");
                    ItemType* newIngredient = ItemType::getTypeById(ingredientTypeStr);
                    ingredientsChar.insert({currentRow[j], newIngredient});
                    ingridients.push_back(newIngredient);
                    pattern[i][j] = newIngredient;
                    if(curRecipeBranch->recipeMap.find(newIngredient) == curRecipeBranch->recipeMap.end()){
                        curRecipeBranch->recipeMap.insert({newIngredient, new RecipeBranch()});
                    }
                    curRecipeBranch = curRecipeBranch->recipeMap[newIngredient];
                }else{
                    pattern[i][j] = ingredientsChar[currentRow[j]];
                }
            }
        }
        curRecipeBranch->recipes.push_back(this);
        // printf("Init result item...\n");
        json resultJson = jsonRecipe.at("result");
        string resultItemStr = (string)resultJson.at("item");
        resultItemStr = resultItemStr.substr(10, resultItemStr.length());
        resultingItem = ItemType::getTypeById(resultItemStr);
        // printf("Init result item finished...\n");
        if(resultJson.contains("count")){
            resultingItemCount = (int) resultJson.at("count");    
        }else{
            resultingItemCount = 1;
        }
    }else if(typeStr.find("smelting") != std::string::npos){
        //printf("Type is smelting...\n");
        cout << "Initialising smelt recipe for " <<  filename << endl;

        type = COOK_RECIPE_TYPE;
        json ingredientsJson = jsonRecipe.at("ingredient");
        if(ingredientsJson.contains("tag")){
            printf("Unsupported recipe with tags... Fix later\n");
            return;
        }
        vector<string> ingrsStr;
        if(ingredientsJson.size() == 1){
            ingrsStr.push_back((string) ingredientsJson.at("item"));
        }else{
            for(int i = 0; i < ingredientsJson.size(); i++){
                ingrsStr.push_back((string) ingredientsJson.at(i).at("item"));
            }
        }
        string resultItemStr = (string)jsonRecipe.at("result");
        resultItemStr = resultItemStr.substr(10, resultItemStr.length());
        ItemType* resultingItem = ItemType::getTypeById(resultItemStr);
        for(int i = 0; i < ingrsStr.size(); i++){
            string ingredientStr = ingrsStr[i];
            // ingredientStr = (string) ingredientsJson.at("item");
            ingredientStr = ingredientStr.substr(10, ingredientStr.length());
            ItemType* ingredientType = ItemType::getTypeById(ingredientStr);
            Recipe::smeltRecipeMap.insert({ingredientType, resultingItem});
        }

    }else if(typeStr.find("crafting_shapeless") != std::string::npos){
        // printf("Crafting 2x2\n");
        type = CRAFT_RECIPE_TYPE_SHAPELESS;
        json ingredientsJson = jsonRecipe.at("ingredients");
        RecipeBranch* curRecipeBranch = Recipe::rootRecipeBranchShapeless;
        for(int i = 0; i < ingredientsJson.size(); i++){
            if(!ingredientsJson.at(i).contains("item")){
                // printf("Unsupported recipe with tags... Fix later\n");
                return;
            }
            string ingredientTypeStr = (string)ingredientsJson.at(i).at("item");
            ingredientTypeStr = ingredientTypeStr.substr(10, ingredientTypeStr.length());
            // cout << "INGREDIENT TYPE: " << ingredientTypeStr  << endl;
            ItemType* newIngredient = ItemType::getTypeById(ingredientTypeStr);
            ingridients.push_back(newIngredient);
        }
        sort(ingridients.begin(), ingridients.end());
        for(int i = 0; i < ingridients.size(); i++){
            ItemType* curIngredient = ingridients[i];
            if(curRecipeBranch->recipeMap.find(curIngredient) == curRecipeBranch->recipeMap.end()){
                curRecipeBranch->recipeMap.insert({curIngredient, new RecipeBranch()});
            }
            curRecipeBranch = curRecipeBranch->recipeMap[curIngredient];
        }
        curRecipeBranch->recipes.push_back(this);
        // printf("Init result item...\n");
        json resultJson = jsonRecipe.at("result");
        string resultItemStr = (string)resultJson.at("item");
        resultItemStr = resultItemStr.substr(10, resultItemStr.length());
        resultingItem = ItemType::getTypeById(resultItemStr);
        // printf("Init result item finished...\n");
        if(resultJson.contains("count")){
            resultingItemCount = (int) resultJson.at("count");    
        }else{
            resultingItemCount = 1;
        }
    }else{
        //printf("Currently unsupported type\n");
    }

}

ItemType* Recipe::craftItemShapeless(vector<ItemType*> inShapeless){
    RecipeBranch* curRecipeBranch = Recipe::rootRecipeBranchShapeless; 
    for(int i = 0; i < inShapeless.size(); i++){
        try{
            curRecipeBranch = curRecipeBranch->recipeMap.at(inShapeless[i]);
        }
        catch (std::out_of_range&  e){
            // printf("No recipe for that pattern \n");
            return NULL;
        }
        // printf("Search through hash tree shapeless %d\n", curRecipeBranch);
    }
    for(int i = 0; i < curRecipeBranch->recipes.size(); i++){
        Recipe* curRecipe = curRecipeBranch->recipes[i];
        if(curRecipe->type == CRAFT_RECIPE_TYPE_SHAPELESS){
            //  printf("Found recipe %s\n", curRecipeBranch->recipes[i]->resultingItem->iconFilePath);
            return curRecipeBranch->recipes[i]->resultingItem;
        }else{
            // printf("Zasto ima vise shapeless sa istim ingr...\n");
        }
    }
    return NULL;
}

ItemType* Recipe::craftItem(ItemType* inPattern[3][3]){
    vector<ItemType*> inIngredients;
    vector<ItemType*> inIngredientsShapeless;

    ItemType* inPatternShifted[3][3] = {    {NULL, NULL, NULL}, \
                                            {NULL, NULL, NULL}, \
                                            {NULL, NULL, NULL}, };
    int emptyRowCount[3] = {0, 0};
    int emptyColCount[3] = {0, 0};
    // printf("Entering the void\n");
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            bool alreadyHasThatIngredient = false;
            if(!inPattern[i][j]){
                emptyRowCount[i]++;
                emptyColCount[j]++;
                continue;
            }
            for(int k = 0; k < inIngredients.size(); k++){
                if(inIngredients[k] == inPattern[i][j]){
                    alreadyHasThatIngredient = true;
                    break;
                }
            }
            if(!alreadyHasThatIngredient){
                // printf("Ingredient found\n");
                inIngredients.push_back(inPattern[i][j]);
            }
            inIngredientsShapeless.push_back(inPattern[i][j]);
        }
    }
    int rowShift = (emptyRowCount[0] == 3);
    rowShift += (rowShift && (emptyRowCount[1] == 3));
    int colShift = (emptyColCount[0] == 3);
    colShift += (colShift && (emptyColCount[1] == 3));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(i < 3 - rowShift){
                inPatternShifted[i][j] = inPattern[i + rowShift][j];
            }else{
                inPatternShifted[i][j] = NULL;
            }
            if(j < 3 - colShift){
                inPatternShifted[i][j] = inPattern[i][j + colShift];
            }else{
                inPatternShifted[i][j] = NULL;
            }
        }
    }
    
    

    // printf("Extracted ingredients\n");
    RecipeBranch* curRecipeBranch = Recipe::rootRecipeBranch; 
    for(int i = 0; i < inIngredients.size(); i++){
        try{
            curRecipeBranch = curRecipeBranch->recipeMap.at(inIngredients[i]);
        }
        catch (std::out_of_range&  e){
            std::cerr << e.what() << std::endl;
            return craftItemShapeless(inIngredientsShapeless);
        }
        
        // printf("Search through hash tree %d\n", curRecipeBranch);
    }
    for(int i = 0; i < curRecipeBranch->recipes.size(); i++){
        // printf("PROBA\n");
        Recipe* curRecipe = curRecipeBranch->recipes[i];
        
        bool isMatched = true;
        // printf("Iterating through recepies\n");
        for(int k = 0; k < 3; k++){
            for(int j = 0; j < 3; j++){
                if(inPatternShifted[k][j] != NULL){
                    // printf("Patern ima [%d][%d]", k, j);
                }
                if(inPatternShifted[k][j] != curRecipe->pattern[k][j]){
                    isMatched = false;
                    break;
                }
            }
            if(!isMatched){
                break;
            }
        }
        if(isMatched){
            // printf("Found recipe %s\n", curRecipeBranch->recipes[i]->resultingItem->iconFilePath);
            return curRecipeBranch->recipes[i]->resultingItem;
        }
    }
    return craftItemShapeless(inIngredientsShapeless);
}
    //make more methods for other types of recepies...
ItemType* Recipe::smeltItem(ItemType* inItem){
    ItemType* result;
    try{
        result = Recipe::smeltRecipeMap.at(inItem);
        return result;
    }
    catch (std::out_of_range&  e){
        std::cerr << e.what() << std::endl;
        return NULL;
    }
    return NULL;
}