#include "render.h"

#include "window_admin.h"
#include "asset_store.h"

#include "constants_window.h"

using program::constants::CELL_LENGTH;
using namespace assets::store::regions;
using assets::store::textures::tiles;

namespace render{
    void tintTilesBy(types::RGB const rgb){
        SDL_SetTextureColorMod(tiles,rgb.r,rgb.g,rgb.b);
    }
    void stopTintingTiles(){
        SDL_SetTextureColorMod(tiles,SDL_ALPHA_OPAQUE,SDL_ALPHA_OPAQUE,SDL_ALPHA_OPAQUE);
    }
    void alphaTilesBy(float const opacity){
        int alpha=SDL_ALPHA_OPAQUE*opacity;
        if(alpha>SDL_ALPHA_OPAQUE)  alpha=SDL_ALPHA_OPAQUE;
        else if(alpha<0)            alpha=0;

        SDL_SetTextureAlphaMod(tiles,alpha);
    }
    void stopAlphaingTiles(){
        SDL_SetTextureAlphaMod(tiles,SDL_ALPHA_OPAQUE);
    }

    void renderUnitLengthSprite(game_logic::items::Coord const& coord, float const xOffset, float const yOffset, SDL_Rect const* const srcRect){
        const SDL_Rect dstRect{
            static_cast<int>((coord.x+xOffset)*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>((coord.y+yOffset)*CELL_LENGTH),
            CELL_LENGTH,
            CELL_LENGTH};
        SDL_RenderCopy(program::renderer,tiles,srcRect,&dstRect);
    }
    void renderGridAlignedSprite(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect){
        float centreX=coord.x+0.5f;
        float centreY=coord.y+0.5f;
        const SDL_Rect dstRect{
            static_cast<int>((centreX-radius)*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>((centreY-radius)*CELL_LENGTH),
            static_cast<int>(2*radius*CELL_LENGTH),
            static_cast<int>(2*radius*CELL_LENGTH)};
        SDL_RenderCopy(program::renderer,tiles,srcRect,&dstRect);
    }
    void renderGridAlignedSprite(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, float opacity){
        alphaTilesBy(opacity);
        renderGridAlignedSprite(coord,radius,srcRect);
        stopAlphaingTiles();
    }
    void renderGridAlignedSpriteRotated(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, double angle){
        float const centreX{coord.x+0.5f};
        float const centreY{coord.y+0.5f};
        const SDL_Rect dstRect{
            static_cast<int>((centreX-radius)*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>((centreY-radius)*CELL_LENGTH),
            static_cast<int>(2*radius*CELL_LENGTH),
            static_cast<int>(2*radius*CELL_LENGTH)};
        SDL_RenderCopyEx(program::renderer,tiles,srcRect,&dstRect,angle,nullptr,SDL_FLIP_NONE);
    }
    void renderGridAlignedSpriteRotated(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, double angle, float opacity){
        alphaTilesBy(opacity);
        renderGridAlignedSpriteRotated(coord,radius,srcRect,angle);
        stopAlphaingTiles();
    }

    void renderColouredItem(game_logic::items::Item::Colour colour, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        renderUnitLengthSprite(coord,xOffset,yOffset,&GEM_TEXTURE_REGIONS[colour]);
    }
    void renderRainbowItem(game_logic::items::Coord const& coord, float xOffset, float yOffset){
        renderUnitLengthSprite(coord,xOffset,yOffset,&RAINBOW_TEXTURE_REGION);
    }

    void renderBombHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset){
        renderUnitLengthSprite(coord,xOffset,yOffset,&BOMB_HALO_TEXTURE_REGION);
    }
    void renderBombHalo(game_logic::items::Coord const& coord, float radius){
        renderGridAlignedSprite(coord,radius,&BOMB_HALO_TEXTURE_REGION);
    }
    void renderBombHalo(game_logic::items::Coord const& coord, float radius, float opacity){
        renderGridAlignedSprite(coord,radius,&BOMB_HALO_TEXTURE_REGION,opacity);
    }

    void renderStarHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset){
        renderUnitLengthSprite(coord,xOffset,yOffset,&STAR_HALO_TEXTURE_REGION);
    }
    void renderStarHalo(game_logic::items::Coord const& coord, float radius){
        renderGridAlignedSprite(coord,radius,&STAR_HALO_TEXTURE_REGION);
    }
    void renderStarHalo(game_logic::items::Coord const& coord, float radius, float opacity){
        renderGridAlignedSprite(coord,radius,&STAR_HALO_TEXTURE_REGION,opacity);
    }
    void renderStarLaser(float centreX, float centreY, float radius, double angle){
        const SDL_Rect dstRect{
            static_cast<int>((centreX-radius)*CELL_LENGTH),//I believe cast is a round towards 0.
            static_cast<int>((centreY-radius)*CELL_LENGTH),
            static_cast<int>(2*radius*CELL_LENGTH),
            static_cast<int>(2*radius*STAR_LASER_ASPECT_RATIO*CELL_LENGTH)};
        //If dstRect is {x,y,w,h} and the rotation point is {px,py}, then the centre of rotation in the screen frame is {x+px,y+py}.
        const SDL_Point rotationCentre{static_cast<int>(radius*CELL_LENGTH),static_cast<int>(radius*CELL_LENGTH)};
        const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
             = &STAR_LASER_TEXTURE_REGION;
        SDL_RenderCopyEx(program::renderer,tiles,srcRect,&dstRect,angle,&rotationCentre,SDL_FLIP_NONE);
    }

    void renderRainbowHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset, types::RGB const colour){
        tintTilesBy(colour);
        renderUnitLengthSprite(coord,xOffset,yOffset,&RAINBOW_HALO_TEXTURE_REGION);
        stopTintingTiles();
    }
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, types::RGB const colour){
        tintTilesBy(colour);
        renderGridAlignedSpriteRotated(coord,radius,&RAINBOW_HALO_TEXTURE_REGION,angle);
        stopTintingTiles();
    }
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, types::RGB const colour, float opacity){
        tintTilesBy(colour);
        renderGridAlignedSpriteRotated(coord,radius,&RAINBOW_HALO_TEXTURE_REGION,angle,opacity);
        stopTintingTiles();
    }
    void renderRainbowHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset, game_logic::items::Item::Colour const colour){
        renderRainbowHaloUnitLength(coord, xOffset, yOffset, assets::store::colours::COLOURS_RGB[colour]);
    }
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, game_logic::items::Item::Colour const colour){
        renderRainbowHalo(coord,radius,angle,assets::store::colours::COLOURS_RGB[colour]);
    }
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, game_logic::items::Item::Colour const colour, float opacity){
        renderRainbowHalo(coord,radius,angle,assets::store::colours::COLOURS_RGB[colour],opacity);
    }
}