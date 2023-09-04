#pragma once

#include "SDL.h"
#include "SDL_image.h"

namespace program{
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    
    bool setupWindow();
    void close();
    void refreshScreen();
}
