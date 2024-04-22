#include "minecraft/ItemType.hpp"

#define CRAFT_RECIPE_TYPE_SHAPED 0
#define CRAFT_RECIPE_TYPE_SHAPELESS 1
#define COOK_RECIPE_TYPE 2

using json = nlohmann::json;
using namespace std;

class Recipe;

class RecipeBranch{
    public:
    map<ItemType*, RecipeBranch*> recipeMap;
    vector<Recipe*> recipes;
};

class Recipe{
    private:
    static RecipeBranch* rootRecipeBranch;
    static RecipeBranch* rootRecipeBranchShapeless;
    static map<ItemType*, ItemType*> smeltRecipeMap;

    static ItemType* craftItemShapeless(vector<ItemType*> in);

    public:
    Recipe();

    static void loadRecipes();
    static ItemType* craftItem(ItemType* inPattern[3][3]);
    //make more methods for other types of recepies...
    static ItemType* smeltItem(ItemType* inItem);

    int type;
    float cookTime;
    int numOfIngredients;
    vector<ItemType*> ingridients;
    ItemType* pattern[3][3] = { {NULL, NULL, NULL},
                                {NULL, NULL, NULL},
                                {NULL, NULL, NULL} };
    ItemType* resultingItem;
    int resultingItemCount;
    void init(string filename);
    char patternStr[3][3] = {   {' ', ' ', ' '},
                                {' ', ' ', ' '},
                                {' ', ' ', ' '} };


    ~Recipe();
};
