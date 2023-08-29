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
        bool isMatchable() const override{return stillSealed;}
        bool isBreakable() const override{return stillSealed;}
        bool readyToDetonate() const override{return (!stillSealed)&&(!detonated);}
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
        bool isMatchable() const override{return stillSealed;}
        bool isBreakable() const override{return stillSealed;}
        bool readyToDetonate() const override{return (!stillSealed)&&(!detonated);}
        bool breakSelf(Colour const cause) override;//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override;//Returns effect responsible for detonating it.
    };
    class Rainbow : public Item{
    private:
        bool stillSealed{true};
        bool detonated{false};
        Colour target{Item::RED};
    public:
        Rainbow(): Item(Item::RED){}
        void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)override;
        bool isMatchable() const override{return false;}
        bool isBreakable() const override{return stillSealed;}
        bool readyToDetonate() const override{return (!stillSealed)&&(!detonated);}
        bool breakSelf(Colour const cause) override;//Returns whether it should now be deleted.
        game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord) override;//Returns effect responsible for detonating it.
    };
}