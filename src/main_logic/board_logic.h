#pragma once

#include "coordinate.h"
#include "item.h"

namespace game_logic::game::board_update{
    bool tryToFindMatches();
    bool tryToMakeFallsHappen();
    bool moveInThatDirectionAndReportWhetherItIsTheSameColour(game_logic::items::Coord &location, game_logic::items::Direction const direction, game_logic::items::Item::Colour const colour);
}