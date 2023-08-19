#include "item_vanilla.h"

#include "constants.h"
#include "window_admin.h"
#include "asset_store.h"

#include <SDL2/SDL.h>

using namespace util::constants;

namespace game_logic::items{
    void ItemVanilla::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        const SDL_Rect dstRect{
            static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
            CELL_LENGTH,
            CELL_LENGTH};
        const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
             = &GEM_TEXTURE_REGIONS[colour];
        SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
    }
}