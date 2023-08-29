#pragma once

#include "coordinate.h"
#include "effect.h"
#include "item_state.h"

namespace game_logic::items{
    class Item{
    public:
        enum Colour{RED,BLUE,YELLOW,GREEN,ORANGE,PURPLE} colour;
        virtual void draw(int dt, game_logic::items::Coord const& coord, float xOffset=0, float yOffset=0)=0;
        virtual ~Item() {};//Need this, otherwise complains (warns) about deleting object of abstract class which has non-virtual destructor when call delete on an Item*.
        virtual ItemState state() const=0;
    protected:
        Item(Colour colour_val): colour(colour_val){}
    public:
        /*
         * The following 2 methods should be protected friends of some methods in game_logic::game, but I couldn't get it to work as required a forward declaration of the Colour enum.
         * breakSelf must assume that it is willing to be broken (state().isBreakable()==true), and return whether it should be destroyed or not.
         * detonateSelf must assume that it is willing to be detonated (state()==WISHES_TO_DETONATE), and return the effect to deal with detonation, or nullptr if the Item is to simply be destroyed.
            * Once it is called, any Item* at coord is going to be destroyed by the calling function. So:
                * If the Effect wants access to the Item, the Item must be claimed from the cell in the grid before returning the Effect*.
                * No new Item should be put at coord before returning the Effect*.
         */
        virtual bool breakSelf(Colour const cause)=0;
        virtual game_logic::effects::Effect* detonateSelf(game_logic::items::Coord const& coord)=0;
    };
    Item::Colour colourFromInt(const int val);
}