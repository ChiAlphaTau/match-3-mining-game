#pragma once

#include "item.h"

namespace game_logic::items{
    class ItemVanilla : public Item{
    public:
        ItemVanilla(Colour colour_val): Item(colour_val){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
        ItemState state() const override{
            return ItemState::CAN_BE_MATCHED_IN_GRID;
        }
        bool breakSelf(Colour const cause) override{return true;}//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override{return nullptr;}//Returns effect responsible for detonating it.
    };
}