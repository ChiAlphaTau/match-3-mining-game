#include "swap.h"

#include "game.h"
#include "default_item_rendering.h"

using game_logic::items::Coord;
using game_logic::items::Item;
using game_logic::effects::Effect;

namespace game_logic::effects{
    SwapFailed::SwapFailed(game_logic::items::Coord const& coord1, game_logic::items::Coord const& coord2):
        startingCoords{coord1,coord2},
        dx(coord2.x - coord1.x),
        dy(coord2.y - coord1.y)
        {
            items[0] = game_logic::game::grid -> claim(coord1);
            items[1] = game_logic::game::grid -> claim(coord2);
        }
    Effect::ExpiryState SwapFailed::update (int dt) {
        progress+=dt;
        if(progress>=2*SWAP_TIME){//If done swapping, put the items back to where they were.
            for(int i=0;i<2;++i){
                game_logic::game::grid -> give(startingCoords[i],items[i]);
                items[i]=NULL;
            }
            return Effect::DONE;
        }
        else{
            return Effect::BUSY;
        }
    }
    void SwapFailed::draw (int dt){
        float proportion{0};
        if(progress<SWAP_TIME){//If swapping outwards, then will be setting coords for that.
            proportion = progress/(float)SWAP_TIME;
        }
        else{//Otherise are on the return, so will be setting coords for that.
            proportion = 2 - progress/(float)SWAP_TIME;
        }
        game_logic::items::render::renderItemDefaultly(items[0],startingCoords[0], proportion*dx, proportion*dy);
        game_logic::items::render::renderItemDefaultly(items[1],startingCoords[1],-proportion*dx,-proportion*dy);
    }
    SwapFailed::~SwapFailed(){
        for(int i=0;i<2;++i){
            if(items[i]!=NULL){
                delete items[i];
                items[i]=NULL;
            }
        }
    }
}