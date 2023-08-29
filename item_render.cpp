#include "item_vanilla.h"
#include "item_specials.h"

#include "constants.h"
#include "window_admin.h"
#include "asset_store.h"

#include <SDL2/SDL.h>

using namespace util::constants;

namespace game_logic::items{
    namespace render{
        void renderColouredItem(game_logic::items::Item::Colour colour, game_logic::items::Coord const& coord, float xOffset, float yOffset){
            const SDL_Rect dstRect{
                static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
                static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
                CELL_LENGTH,
                CELL_LENGTH};
            const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
                 = &GEM_TEXTURE_REGIONS[colour];
            SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
        }
        void renderRainbowItem(game_logic::items::Coord const& coord, float xOffset, float yOffset){
            const SDL_Rect dstRect{
                static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
                static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
                CELL_LENGTH,
                CELL_LENGTH};
            const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
                 = &RAINBOW_TEXTURE_REGION;
            SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
        }
        void renderBombHalo(game_logic::items::Coord const& coord, float xOffset, float yOffset){
            const SDL_Rect dstRect{
                static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
                static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
                CELL_LENGTH,
                CELL_LENGTH};
            const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
                 = &BOMB_HALO_TEXTURE_REGION;
            SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
        }
        void renderStarHalo(game_logic::items::Coord const& coord, float xOffset, float yOffset){
            const SDL_Rect dstRect{
                static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
                static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
                CELL_LENGTH,
                CELL_LENGTH};
            const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
                 = &STAR_HALO_TEXTURE_REGION;
            SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
        }
    }
    void ItemVanilla::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Bomb::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderBombHalo(coord,xOffset,yOffset);
        if(stillSealed)
            render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Star::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderStarHalo(coord,xOffset,yOffset);
        if(stillSealed)
            render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Rainbow::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        if(stillSealed)
            render::renderRainbowItem(coord,xOffset,yOffset);
    }
}