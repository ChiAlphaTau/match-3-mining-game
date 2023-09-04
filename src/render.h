#pragma once

#include "rgb_union.h"
#include "coordinate.h"
#include "item.h"

#include <SDL2/SDL.h>

namespace render{
    void tintTilesBy(types::RGB const rgb);
    void stopTintingTiles();
    void alphaTilesBy(float const opacity);
    void stopAlphaingTiles();

    void renderUnitLengthSprite(game_logic::items::Coord const& coord, float const xOffset, float const yOffset, SDL_Rect const* const srcRect);
    void renderGridAlignedSprite(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect);
    void renderGridAlignedSprite(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, float opacity);
    void renderGridAlignedSpriteRotated(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, double angle);
    void renderGridAlignedSpriteRotated(game_logic::items::Coord const& coord, float radius, SDL_Rect const* const srcRect, double angle, float opacity);

    void renderColouredItem(game_logic::items::Item::Colour colour, game_logic::items::Coord const& coord, float xOffset, float yOffset);
    void renderRainbowItem(game_logic::items::Coord const& coord, float xOffset, float yOffset);

    void renderBombHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset);
    void renderBombHalo(game_logic::items::Coord const& coord, float radius);
    void renderBombHalo(game_logic::items::Coord const& coord, float radius, float opacity);

    void renderStarHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset);
    void renderStarHalo(game_logic::items::Coord const& coord, float radius);
    void renderStarHalo(game_logic::items::Coord const& coord, float radius, float opacity);
    void renderStarLaser(float centreX, float centreY, float radius, double angle);

    void renderRainbowHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset, types::RGB const colour);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, types::RGB const colour);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, types::RGB const colour, float opacity);
    void renderRainbowHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset, game_logic::items::Item::Colour const colour);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, game_logic::items::Item::Colour const colour);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, game_logic::items::Item::Colour const colour, float opacity);
}