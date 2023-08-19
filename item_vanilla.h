#pragma once

#include "item.h"

namespace game_logic::items{
    class ItemVanilla : public Item{
    public:
        ItemVanilla(Colour colour_val): Item(colour_val){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
    };
}