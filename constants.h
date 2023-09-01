#pragma once

#include <SDL2/SDL.h>

#define NEWLINE '\n'

namespace util{
    namespace constants{
        const int CELL_LENGTH=64;
        const int CELL_LENGTH_HALF=CELL_LENGTH/2;
        const int CELL_COUNT_HORIZONTAL=10;
        const int CELL_COUNT_VERTICAL=10;
        const int WINDOW_WIDTH=CELL_LENGTH*CELL_COUNT_HORIZONTAL;
        const int WINDOW_HEIGHT=CELL_LENGTH*CELL_COUNT_VERTICAL;
        const int NUMBER_OF_COLOURS=6;

        const int GEM_TEXTURE_LENGTH=192;
        const SDL_Rect GEM_TEXTURE_REGIONS[NUMBER_OF_COLOURS]{
            SDL_Rect{0                   ,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
            SDL_Rect{1*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
            SDL_Rect{2*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
            SDL_Rect{3*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
            SDL_Rect{2*GEM_TEXTURE_LENGTH,1*GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
            SDL_Rect{3*GEM_TEXTURE_LENGTH,1*GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
        };
        const SDL_Rect BOMB_HALO_TEXTURE_REGION=
            SDL_Rect{1*GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH};
        const SDL_Rect STAR_HALO_TEXTURE_REGION=
            SDL_Rect{996,0,223,222};
        const SDL_Rect STAR_LASER_TEXTURE_REGION=
            SDL_Rect{1219,0,59,249};
        const float STAR_LASER_ASPECT_RATIO=
            static_cast<float>(STAR_LASER_TEXTURE_REGION.h)/static_cast<float>(STAR_LASER_TEXTURE_REGION.w);
        const SDL_Rect RAINBOW_HALO_TEXTURE_REGION=
            SDL_Rect{768,0,228,228};
        const SDL_Rect RAINBOW_TEXTURE_REGION=
            SDL_Rect{0,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH};
    }
}