#include "item_specials.h"

namespace game_logic::items{
    bool Star::breakSelf(Colour const cause){
        stillSealed=false;
        return false;
    }
    game_logic::effects::Effect* Star::detonateSelf(game_logic::items::Coord const& coord){//Returns effect responsible for detonating it.
        detonated=true;
        return nullptr;
    }
}