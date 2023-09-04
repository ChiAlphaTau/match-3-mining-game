#pragma once

#include "rgb_union.h"

#include "constants_colour_count.h"

#include "SDL.h"

namespace assets{
    namespace store{
        namespace textures{
            extern SDL_Texture* tiles;
        }
        namespace colours{//Found by sampling the colour block that includes the pixel at (100,70) in {Spritesheet/spritesheet_tilesColour.png} from Puzzle Pack II (see {rundir/images/credit.txt}).
            const render::types::RGB COLOURS_RGB[game_logic::constants::NUMBER_OF_COLOURS+1]{
                0xc83e3e,0x1ea7e1,0xffcc00,0x80be1f,0xe86a17,0xff99cc,
                0xffffff
            };
        }
        namespace regions{
            const int GEM_TEXTURE_LENGTH=192;
            const SDL_Rect RAINBOW_TEXTURE_REGION=
                SDL_Rect{0,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH};
            const SDL_Rect GEM_TEXTURE_REGIONS[game_logic::constants::NUMBER_OF_COLOURS+1]{
                SDL_Rect{0                   ,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                SDL_Rect{1*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                SDL_Rect{2*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                SDL_Rect{3*GEM_TEXTURE_LENGTH,0                   ,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                SDL_Rect{2*GEM_TEXTURE_LENGTH,1*GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                SDL_Rect{3*GEM_TEXTURE_LENGTH,1*GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH,GEM_TEXTURE_LENGTH},
                RAINBOW_TEXTURE_REGION,
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
        }
    }
}