#pragma once

#include <SDL2/SDL.h>

namespace game_logic{
    namespace game{
        bool setup();
        void close();
        void handleEvent(const SDL_Event& event, const int dt);
        void update(const int dt);
        void draw(const int dt);
        //TODO:cursor stuff.
        //TODO:effect stuff.
        //TODO:grid stuff.
    }
}