#pragma once

#include "item.h"
#include "constants.h"
#include "coordinate.h"

using game_logic::items::Item;

namespace game_logic{
    class Grid{
    private:
        Item* items[util::constants::CELL_COUNT_HORIZONTAL][util::constants::CELL_COUNT_VERTICAL];
    public:
        Grid();
        ~Grid();
        Item* peek(Coord coord);
        Item* claim(Coord coord);
        void destroy(Coord coord);
        void give(Coord coord, Item* item);
    };
}