#include "item_vanilla.h"
#include "item_specials.h"

#include "render.h"

namespace game_logic::items{
    void ItemVanilla::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Bomb::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderBombHaloUnitLength(coord,xOffset,yOffset);
        if(stillSealed)
            render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Star::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        render::renderStarHaloUnitLength(coord,xOffset,yOffset);
        if(stillSealed)
            render::renderColouredItem(colour,coord,xOffset,yOffset);
    }
    void Rainbow::draw(int dt, game_logic::items::Coord const& coord, float xOffset, float yOffset){
        if(stillSealed){
            render::renderRainbowHaloUnitLength(coord,xOffset,yOffset,render::types::RGB{0xffffff});
            render::renderRainbowItem(coord,xOffset,yOffset);
        }
        else{
            render::renderRainbowHaloUnitLength(coord,xOffset,yOffset,target);
        }
    }
}