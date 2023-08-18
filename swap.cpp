#include "swap.h"

#include <iostream>

using game_logic::items::Coord;
using game_logic::items::Item;
using game_logic::effects::Effect;

namespace game_logic::effects{
    SwapFailed::SwapFailed(Item* item1, Item* item2):
        items{item1,item2},
        startingCoords{Coord(item1->x,item1->y),Coord(item2->x,item2->y)},
        dx(item2->x - item1->x),
        dy(item2->y - item1->y)
        {}
    Effect::ExpiryState SwapFailed::update (int dt) {
        progress+=dt;
        float proportion{0};
        if(progress>=2*SWAP_TIME){//If done swapping, put the items back to where they were.
            items[0]->x=startingCoords[0].x;    items[0]->y=startingCoords[0].y;
            items[1]->x=startingCoords[1].x;    items[1]->y=startingCoords[1].y;
            return Effect::DONE;
        }
        else if(progress<SWAP_TIME){//Otherwise if swapping outwards, then will be setting coords for that.
            proportion = progress/(float)SWAP_TIME;
        }
        else{//Otherise are on the return, so will be setting coords for that.
            proportion = 2 - progress/(float)SWAP_TIME;
        }
        items[0]->x=startingCoords[0].x+proportion*dx;  items[0]->y=startingCoords[0].y+proportion*dy;
        items[1]->x=startingCoords[1].x-proportion*dx;  items[1]->y=startingCoords[1].y-proportion*dy;
        return Effect::BUSY;
    }
    void SwapFailed::draw (int dt){
        //TODO.
    }
    SwapFailed::~SwapFailed(){
        std::cout << "SwapFailed destructor called.\n";
    }
}