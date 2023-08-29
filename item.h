#pragma once

#include "coordinate.h"
#include "effect.h"

namespace game_logic::items{
    class Item{
    public:
        enum Colour{RED,BLUE,YELLOW,GREEN,ORANGE,PURPLE} colour;
        virtual void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)=0;
        virtual ~Item() {};//Need this, otherwise complains (warns) about deleting object of abstract class which has non-virtual destructor when call delete on an Item*.
        /*
         * Options are:
         * Matchable? | Breakable? | ReadyToDetonate? | Desription
         * -----------+------------+------------------+-----------
         * Yes        |Yes         |No                |In grid, waiting to be matched.
         * No         |Yes         |No                |Colourless, but can be destroyed (rainbow).
         * No         |No          |Yes               |Waiting to go off.
         * TODO:replace 3 interconnected bools with 1 enum.
         */
        virtual bool isMatchable() const=0;
        virtual bool isBreakable() const=0;
        virtual bool readyToDetonate() const=0;
        //Returns whether it should now be deleted. If it returns false, then detonateSelf MUST return non-null.
        virtual bool breakSelf(Colour const cause)=0;
        //Returns effect responsible for detonating it. That effect must have removed the item before being returned if it wishes to have access to it, otherwise it shall be destroyed.
        //Also, don't add an item at coord either, as that shall be destroyed.
        virtual game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord)=0;
    protected:
        Item(Colour colour_val): colour(colour_val){}
    };
    Item::Colour colourFromInt(const int val);
}