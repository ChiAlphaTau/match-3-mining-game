#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace program{
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    
    bool setupWindow();
    void close();
    void refreshScreen();
}
