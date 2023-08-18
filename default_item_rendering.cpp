#include "default_item_rendering.h"

#include "constants.h"
#include "window_admin.h"
#include "asset_store.h"

#include <SDL2/SDL.h>

using namespace util::constants;

namespace game_logic::items::render{
    void renderItemDefaultly(Item* const item){
        SDL_Rect dstRect{
            static_cast<int>(item->x*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>(item->y*CELL_LENGTH),
            CELL_LENGTH,
            CELL_LENGTH};
        const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
             = &GEM_TEXTURE_REGIONS[item->colour];
        SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
    }
}