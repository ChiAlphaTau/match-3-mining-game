#include "item_specials.h"

namespace game_logic::items{
    bool Rainbow::breakSelf(Colour const cause){
        stillSealed=false;
        target=cause;
        return false;
    }
    game_logic::effects::Effect* Rainbow::detonateSelf(game_logic::items::Coord const& coord){//Returns effect responsible for detonating it.
        detonated=true;
        return nullptr;
    }
}