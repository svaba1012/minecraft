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
    string fullTexturePath ="./assets/extern_minecraft_assets/assets/minecraft/textures/" + texturePath + ".png";
      char* fullTexturePathChar = new char[fullTexturePath.length() + 1];
      strcpy(fullTexturePathChar, fullTexturePath.c_str()); 
    return new Texture(fullTexturePathChar);
}

bool BlockType::readJsonBool(char* fieldName){
    return (bool) (((string) this->blockData.at(fieldName)).compare("Yes") == 0);
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

    this->hardness = (float) this->blockData.at("hardness");
    this->mineableWithAxe = readJsonBool("tag_mineable_with_axe");
    this->mineableWithPickaxe = readJsonBool("tag_mineable_with_pickaxe");
    this->mineableWithHoe = readJsonBool("tag_mineable_with_hoe");
    this->mineableWithShovel = readJsonBool("tag_mineable_with_shovel");
    this->needsStoneTool = readJsonBool("tag_needs_stone_tool");
    this->needsIronTool = readJsonBool("tag_needs_iron_tool");
    this->needsDiamondTool = readJsonBool("tag_needs_diamond_tool");



    

/*
"block":"Activator Rail","affects_movement_speed":"No","blast_resistance":0.7,"block_entity":"No","block_entity_data":"No","block_render_type":"cutout","blocks_beacon_beam":"No","blocks_motion":"No","blocks_skylight":{"waterlogged: false":"No","waterlogged: true":"Yes"},"bottom_face_has_collision":"No","bottom_face_has_full_square":"No","bottom_face_has_small_square":"No","burn_odds":0,"collision_bottom_all":"Not Applicable","collision_bottom_external":"Not Applicable","comparator_output":"No","conductive":"No","connects_to_panes":"No","dragon_immune":"No","emits_power":"No","exists_as_item":"Yes","falling_block":"No","flammable":"No","fluid_render_type":{"waterlogged: false":"solid","waterlogged: true":"translucent"},"full_cube":"No","generates_in_structures":"No","gets_flushed":"No","gets_random_ticked":"No","hardness":0.7,"has_collision":"No","height_all":"Not Applicable","height_external":"Not Applicable","ignite_odds":0,"ignited_by_lava":"No","inflicts_damage":"No","instant_block_updater":"Yes","instant_mineable":"With eff. V Diamond","instant_shape_updater":"No","instant_updater":"Yes","instrument":"HARP","is_opaque_full_cube":"No","kills_grass":{"waterlogged: false":"No","waterlogged: true":"Yes"},"luminance":"No","map_color":"{{mapColor|NONE}}","material":"DECORATION","material_blocks_light":"No","material_blocks_movement":"No","material_is_burnable":"No","material_is_liquid":"No","material_is_replaceable":"No","material_is_solid":"No","movable":"Yes","numerical_id":"None","obstructs_cactus":"No","opacity":{"waterlogged: false":0,"waterlogged: true":1},"opaque":"No","placement_condition":".top_face_has_rim()","redirects_redstone":"No","replaceable":"No","sends_comparator_updates":"No","shape_update_from_environment":"On reaction","shape_update_on_interaction":"No","shape_update_when_powered":"Yes","side_face_has_collision":"No","side_face_has_full_square":"No","solid":"No","spawnable":"No","sticky":"Yes","suffocates_mobs":"No","supports_redstone_dust":"No","survival_available":"Creatable","tag_acacia_logs":"No","tag_all_hanging_signs":"No","tag_all_signs":"No","tag_ancient_city_replaceable":"No","tag_animals_spawnable_on":"No","tag_anvil":"No","tag_axolotls_spawnable_on":"No","tag_azalea_grows_on":"No","tag_azalea_root_replaceable":"No","tag_bamboo_blocks":"No","tag_bamboo_plantable_on":"No","tag_banners":"No","tag_base_stone_nether":"No","tag_base_stone_overworld":"No","tag_beacon_base_blocks":"No","tag_beds":"No","tag_bee_growables":"No","tag_beehives":"No","tag_big_dripleaf_placeable":"No","tag_birch_logs":"No","tag_buttons":"No","tag_campfires":"No","tag_candle_cakes":"No","tag_candles":"No","tag_cauldrons":"No","tag_cave_vines":"No","tag_ceiling_hanging_signs":"No","tag_cherry_logs":"No","tag_climbable":"No","tag_coal_ores":"No","tag_combination_step_sound_blocks":"No","tag_completes_find_tree_tutorial":"No","tag_convertable_to_mud":"No","tag_copper_ores":"No","tag_coral_blocks":"No","tag_coral_plants":"No","tag_corals":"No","tag_crimson_stems":"No","tag_crops":"No","tag_crystal_sound_blocks":"No","tag_dampens_vibrations":"No","tag_dark_oak_logs":"No","tag_dead_bush_may_place_on":"No","tag_deepslate_ore_replaceables":"No","tag_diamond_ores":"No","tag_dirt":"No","tag_doors":"No","tag_dragon_immune":"No","tag_dragon_transparent":"No","tag_dripstone_replaceable":"No","tag_emerald_ores":"No","tag_enchantment_power_provider":"No","tag_enchantment_power_transmitter":"No","tag_enderman_holdable":"No","tag_fall_damage_resetting":"No","tag_features_cannot_replace":"No","tag_fence_gates":"No","tag_fences":"No","tag_fire":"No","tag_flower_pots":"No","tag_flowers":"No","tag_foxes_spawnable_on":"No","tag_frog_prefer_jump_to":"No","tag_frogs_spawnable_on":"No","tag_geode_invalid_blocks":"No","tag_goats_spawnable_on":"No","tag_gold_ores":"No","tag_guarded_by_piglins":"No","tag_hoglin_repellents":"No","tag_ice":"No","tag_impermeable":"No","tag_infiniburn_end":"No","tag_infiniburn_nether":"No","tag_infiniburn_overworld":"No","tag_inside_step_sound_blocks":"No","tag_invalid_spawn_inside":"No","tag_iron_ores":"No","tag_jungle_logs":"No","tag_lapis_ores":"No","tag_lava_pool_stone_cannot_replace":"No","tag_leaves":"No","tag_logs":"No","tag_logs_that_burn":"No","tag_lush_ground_replaceable":"No","tag_maintains_farmland":"No","tag_mangrove_logs":"No","tag_mangrove_logs_can_grow_through":"No","tag_mangrove_roots_can_grow_through":"No","tag_mineable_with_axe":"No","tag_mineable_with_hoe":"No","tag_mineable_with_pickaxe":"Yes","tag_mineable_with_shovel":"No","tag_mooshrooms_spawnable_on":"No","tag_moss_replaceable":"No","tag_mushroom_grow_block":"No","tag_needs_diamond_tool":"No","tag_needs_iron_tool":"No","tag_needs_stone_tool":"No","tag_nether_carver_replaceables":"No","tag_nylium":"No","tag_oak_logs":"No","tag_occludes_vibration_signals":"No","tag_overworld_carver_replaceables":"No","tag_overworld_natural_logs":"No","tag_parrots_spawnable_on":"No","tag_piglin_repellents":"No","tag_planks":"No","tag_polar_bears_spawnable_on_alternate":"No","tag_portals":"No","tag_pressure_plates":"No","tag_prevent_mob_spawning_inside":"Yes","tag_rabbits_spawnable_on":"No","tag_rails":"Yes","tag_redstone_ores":"No","tag_replaceable":"No","tag_replaceable_by_trees":"No","tag_sand":"No","tag_saplings":"No","tag_sculk_replaceable":"No","tag_sculk_replaceable_world_gen":"No","tag_shulker_boxes":"No","tag_signs":"No","tag_slabs":"No","tag_small_dripleaf_placeable":"No","tag_small_flowers":"No","tag_smelts_to_glass":"No","tag_snaps_goat_horn":"No","tag_sniffer_diggable_block":"No","tag_sniffer_egg_hatch_boost":"No","tag_snow":"No","tag_snow_layer_can_survive_on":"No","tag_snow_layer_cannot_survive_on":"No","tag_soul_fire_base_blocks":"No","tag_soul_speed_blocks":"No","tag_spruce_logs":"No","tag_stairs":"No","tag_standing_signs":"No","tag_stone_bricks":"No","tag_stone_buttons":"No","tag_stone_ore_replaceables":"No","tag_stone_pressure_plates":"No","tag_strider_warm_blocks":"No","tag_sword_efficient":"No","tag_tall_flowers":"No","tag_terracotta":"No","tag_trail_ruins_replaceable":"No","tag_trapdoors":"No","tag_underwater_bonemeals":"No","tag_unstable_bottom_center":"No","tag_valid_spawn":"No","tag_vibration_resonators":"No","tag_wall_corals":"No","tag_wall_hanging_signs":"No","tag_wall_post_override":"No","tag_wall_signs":"No","tag_walls":"No","tag_warped_stems":"No","tag_wart_blocks":"No","tag_wither_immune":"No","tag_wither_summon_base_blocks":"No","tag_wolves_spawnable_on":"No","tag_wooden_buttons":"No","tag_wooden_doors":"No","tag_wooden_fences":"No","tag_wooden_pressure_plates":"No","tag_wooden_slabs":"No","tag_wooden_stairs":"No","tag_wooden_trapdoors":"No","tag_wool":"No","tag_wool_carpets":"No","top_face_has_collision":"No","top_face_has_full_square":"No","top_face_has_rim":"No","top_face_has_small_square":"No","variants":"Activator Rail","waterloggable":"Yes","width_all":"Not Applicable","width_external":"Not Applicable","wiki_page":"https://minecraft.wiki/w/Activator_Rail","wither_block_break_immune":"No","wither_skull_immune":"No","comparator2":"No","requires_silk_touch":"No","intended_tool":"Pickaxe","xp_dropped_when_mined":"None"}
*/





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

        std::ifstream f("./assets/extern_minecraft_assets/assets/minecraft/models/block/" + curVariant + ".json");
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