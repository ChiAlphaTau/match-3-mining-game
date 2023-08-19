#pragma once

#include "item.h"
#include "coordinate.h"

namespace game_logic::items::render{
    void renderItemDefaultly(Item* const item, Coord const& coord, float xOffset=0.f, float yOffset=0.f);
}