#include "swap.h"

#include "game.h"

using game_logic::items::Coord;
using game_logic::items::Item;
using game_logic::effects::Effect;

namespace game_logic::effects{
    /*
     * Tell C++ to compile for the cases of template paramaters SUCCESSFUL being true or false.
     * If I understand correctly, if I don't tell C++ to do that, when swap.cpp compiles, the compiler "doesn't actually compile anything", as it isn't told of any template paramaters that it should be compiling for.
     * Then if in some other file try to use the class with concrete template paramater, say false, then when that other file compiles:
         * It can't find compiled code for Swap<false>, as none was made when swap.cpp was compiled.
         * It can't compile it itself, as it doesn't have access to the code from swap.cpp.
     * So get error:
         * undefined reference to `game_logic::effects::Swap<false>::Swap(game_logic::items::Coord const&, game_logic::items::Coord const&)'
    */
    template class Swap<false>;
    template class Swap<true>;

    template <bool SUCCESSFUL> Swap<SUCCESSFUL>::Swap(game_logic::items::Coord const& coord1, game_logic::items::Coord const& coord2):
        startingCoords{coord1,coord2},
        dx(coord2.x - coord1.x),
        dy(coord2.y - coord1.y)
        {
            items[0] = game_logic::game::grid -> claim(coord1);
            items[1] = game_logic::game::grid -> claim(coord2);
        }
    template<> Effect::ExpiryState Swap<false>::update (int dt) {
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
    template<> Effect::ExpiryState Swap<true>::update (int dt) {
        progress+=dt;
        if(progress>SWAP_TIME){//If done swapping.
            game_logic::items::Item::Colour colour[2];
            bool activateable[2];
            for(int i=0;i<2;++i){//Then put the items in their new place.
                colour[i]=items[i]->colour;
                activateable[i]=items[i]->state()==game_logic::items::ItemState::CAN_BE_ACTIVATED_IN_GRID;
                game_logic::game::grid -> give(startingCoords[1-i],items[i]);
            }
            for(int i=0;i<2;++i){//Activating any activateable items.
                if(activateable[i]){
                    game_logic::game::breakBreakableItemInGrid(items[i],colour[1-i],startingCoords[1-i]);
                }
            }
            for(int i=0;i<2;++i)    items[i]=nullptr;//Then forgetting about the items.
            return Effect::DONE;
        }
        else{
            return Effect::BUSY;
        }
    }
    template <bool SUCCESSFUL> void Swap<SUCCESSFUL>::draw (int dt){
        float proportion{0};
        if(progress<SWAP_TIME){//If swapping outwards, then will be setting coords for that.
            proportion = progress/(float)SWAP_TIME;
        }
        else{//Otherise are on the return, so will be setting coords for that.
            proportion = 2 - progress/(float)SWAP_TIME;
        }
        items[0]->draw(dt,startingCoords[0], proportion*dx, proportion*dy);
        items[1]->draw(dt,startingCoords[1],-proportion*dx,-proportion*dy);
    }
    template <bool SUCCESSFUL> Swap<SUCCESSFUL>::~Swap(){
        for(int i=0;i<2;++i){
            if(items[i]!=NULL){
                delete items[i];
                items[i]=NULL;
            }
        }
    }
}