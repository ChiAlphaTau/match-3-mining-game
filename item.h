#pragma once

namespace game_logic::items{
    class Item{
    public:
        float x;//TODO: replace (x,y) with Coord?
        float y;
        enum Colour{RED,BLUE,YELLOW,GREEN,ORANGE,PURPLE} colour;
        Item(int x_val, int y_val, Colour colour_val): x(x_val), y(y_val), colour(colour_val){}
    };
    Item::Colour colourFromInt(const int val);
}