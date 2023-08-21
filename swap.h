#pragma once

#include "effect.h"
#include "item.h"
#include "coordinate.h"

namespace game_logic::effects{
    template <bool SUCCESSFUL> class Swap: public Effect{
    private:
        game_logic::items::Coord startingCoords[2];
        float dx;
        float dy;
        game_logic::items::Item* items[2];
        int progress{0};
        const int SWAP_TIME=500;
    public:
        Swap(game_logic::items::Coord const& coord1, game_logic::items::Coord const& coord2);
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        ~Swap() override;
    };
}