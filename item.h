#pragma once

namespace game_logic::items{
    class Item{
    public:
        enum Colour{RED,BLUE,YELLOW,GREEN,ORANGE,PURPLE} colour;
        Item(Colour colour_val): colour(colour_val){}
    };
    Item::Colour colourFromInt(const int val);
}