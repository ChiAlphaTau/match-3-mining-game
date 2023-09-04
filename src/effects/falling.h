#pragma once

#include "effect.h"
#include "item.h"
#include "coordinate.h"

#include <vector>

namespace game_logic::effects{
    class Fall:public Effect{
    private:
        game_logic::items::Coord const bottomCoord;
        int const itemCount;//Should have that items contains itemCount items.
        std::vector<game_logic::items::Item*> items;
        const float a=3e-5f;//Acceleration of free fall, used for suvat. Should be positive.
        int t{0};
        float s;//Vertical displacement of bottom item relative to final destination.
        void updateS();//Update s based on t (using suvat). One should call this each time t is modified.
    public:
        Fall(game_logic::items::Coord const& bottomCoordVal, const int itemCountVal, std::vector<game_logic::items::Item*> const& fallingItems);
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        ~Fall() override;
    };
}