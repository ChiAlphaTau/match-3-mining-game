#pragma once

#include "item.h"

namespace game_logic::items{
    class Bomb : public Item{
    private:
        bool stillSealed{true};
        bool detonated{false};
    public:
        Bomb(Colour colour_val): Item(colour_val){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
        ItemState state() const override{
            if(stillSealed)     return ItemState::CAN_BE_MATCHED_IN_GRID;
            else if(!detonated) return ItemState::WISHES_TO_DETONATE;
            else                return ItemState::DETONATED;
        }
        bool breakSelf(Colour const cause) override;//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override;//Returns effect responsible for detonating it.
    };
    class Star : public Item{
    private:
        bool stillSealed{true};
        bool detonated{false};
    public:
        Star(Colour colour_val): Item(colour_val){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
        ItemState state() const override{
            if(stillSealed)     return ItemState::CAN_BE_MATCHED_IN_GRID;
            else if(!detonated) return ItemState::WISHES_TO_DETONATE;
            else                return ItemState::DETONATED;
        }
        bool breakSelf(Colour const cause) override;//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override;//Returns effect responsible for detonating it.
    };
    class Rainbow : public Item{
    private:
        bool stillSealed{true};
        bool detonated{false};
        Colour target{Item::RAINBOW};
    public:
        Rainbow(): Item(Item::RAINBOW){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
        ItemState state() const override{
            if(stillSealed)     return ItemState::CAN_BE_ACTIVATED_IN_GRID;
            else if(!detonated) return ItemState::WISHES_TO_DETONATE;
            else                return ItemState::DETONATED;
        }
        bool breakSelf(Colour const cause) override;//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override;//Returns effect responsible for detonating it.
    };
}