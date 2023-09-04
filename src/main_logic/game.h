#pragma once

#include "grid.h"
#include "effect.h"

#include "SDL.h"

namespace game_logic{
    namespace game{
        extern Grid* grid;
        bool setup();
        void close();
        void handleEvent(const SDL_Event& event, const int dt);
        void update(const int dt);
        void draw(const int dt);
        void pushEffect(game_logic::effects::Effect* effect);
        enum BoardState{
            AWAITING_INPUT,//Waiting for user to make a swap.
            TEMPORARILY_NOT_ACCEPTING_INPUT,//Sort of awaiting user input, but will ignore it. E.g. doing the graphics for an invalid swap.
            RESOLVING,//Deleting items, explosions going off, etc.
            MOVING,//To be followed by a check for whether should resolve. E.g. items falling&spawning, or doing the graphics for a valid swap.
        };
        BoardState getBoardState();
        //Return value is whether item was deleted. So if it returns true, item has been deleted. Otherwise, it still exists at coord.
        bool breakBreakableItemInGrid(game_logic::items::Item* item, game_logic::items::Item::Colour cause, game_logic::items::Coord const coord);
        //Return value is whether item was deleted. So if it returns true, item has been deleted. Otherwise, it still exists (so is the responsibility of the owner still).
        bool breakBreakableItemNotInGrid(game_logic::items::Item* item, game_logic::items::Item::Colour cause);
        //No return value - you have always lost ownership of the item - either destroyed, or taken over by the destroying Effect.
        void detonatePrimedItemInGrid(game_logic::items::Item* item, game_logic::items::Coord const coord);
    }
}