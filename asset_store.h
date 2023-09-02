#pragma once

#include "rgb_union.h"
#include "constants.h"

#include <SDL2/SDL.h>

namespace assets{
    namespace store{
        extern SDL_Texture* tiles;
        namespace colours{//Found by sampling the colour block that includes the pixel at (100,70) in {Spritesheet/spritesheet_tilesColour.png} from Puzzle Pack II (see {rundir/images/credit.txt}).
            const render::types::RGB COLOURS_RGB[util::constants::NUMBER_OF_COLOURS+1]{
                0xc83e3e,0x1ea7e1,0xffcc00,0x80be1f,0xe86a17,0xff99cc,
                0xffffff
            };
        }
    }
}