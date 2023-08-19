#pragma once

#include "coordinate.h"

namespace game_logic::items{
    class Item{
    public:
        enum Colour{RED,BLUE,YELLOW,GREEN,ORANGE,PURPLE} colour;
        virtual void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)=0;
        virtual ~Item() {};//Need this, otherwise complains (warns) about deleting object of abstract class which has non-virtual destructor when call delete on an Item*.
    protected:
        Item(Colour colour_val): colour(colour_val){}
    };
    Item::Colour colourFromInt(const int val);
}