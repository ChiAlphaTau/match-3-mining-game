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
        //TODO:cursor stuff.
        void pushEffect(game_logic::effects::Effect* effect);
    }
}