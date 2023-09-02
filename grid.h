#pragma once

#include "item.h"
#include "coordinate.h"

#include "constants_grid.h"

using game_logic::items::Item;
using game_logic::items::Coord;

namespace game_logic{
    class Grid{
    private:
        Item* items[game_logic::constants::CELL_COUNT_HORIZONTAL][game_logic::constants::CELL_COUNT_VERTICAL];
    public:
        Grid();
        ~Grid();
        Item* peek(Coord const& coord);
        Item* claim(Coord const& coord);
        void destroy(Coord const& coord);
        void give(Coord const& coord, Item* item);
    };
}