#pragma once

#include "grid.h"
#include "effect.h"

#include <SDL2/SDL.h>

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
    }
}