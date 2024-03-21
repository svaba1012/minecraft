#pragma once
#include <vector>

#include "gameLib/ui/GameUI2D.hpp"
#include "Window.hpp"


using namespace std;



class CharacterStatusUI: public GameUI2D{
private:

public:
    
    CharacterStatusUI();
    
    void draw(struct nk_context* ctx);
    
    ~CharacterStatusUI();
};