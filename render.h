#pragma once

#include "coordinate.h"
#include "item.h"

#include <SDL2/SDL.h>

namespace render{
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

    void renderRainbowHaloUnitLength(game_logic::items::Coord const& coord, float xOffset, float yOffset, Uint8 const r, Uint8 const g, Uint8 const b);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, Uint8 const r, Uint8 const g, Uint8 const b);
    void renderRainbowHalo(game_logic::items::Coord const& coord, float const radius, double const angle, Uint8 const r, Uint8 const g, Uint8 const b, float opacity);
}