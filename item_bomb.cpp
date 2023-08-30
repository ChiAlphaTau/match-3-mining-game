#include "item_specials.h"

#include "effect.h"
#include "game.h"
#include "window_admin.h"
#include "asset_store.h"
#include "constants.h"

#include <cmath>

using util::constants::CELL_LENGTH;
using util::constants::BOMB_HALO_TEXTURE_REGION;
using game_logic::items::Direction;

namespace game_logic::effects{
    //Effect to make bomb implode a bit, then explode, attempting to break the surrounding 8 items, then fading.
    class HandleBomb:public Effect{
    private:
        const float INITIAL_RADIUS{0.5f};
        const float TRIGGER_RADIUS{1.25f};//Radius at which destroy horizontally/vertically adjacent items (for diagonally adjacent, it is sqrt(2)*this).

        const float TARGET_RADIUS_IMPLODE{0.25f};
        const int TOTAL_TIME_IMPLODE{1000};
        const float RADIAL_VELOCITY_IMPLODE{(INITIAL_RADIUS-TARGET_RADIUS_IMPLODE)/TOTAL_TIME_IMPLODE};

        const float TARGET_RADIUS_EXPLODE{2.f};
        const int TOTAL_TIME_EXPLODE{500};
        const float RADIAL_VELOCITY_EXPLODE{(TARGET_RADIUS_EXPLODE-TARGET_RADIUS_IMPLODE)/TOTAL_TIME_EXPLODE};

        const int FADE_TIME{1000};
    private:
        Coord const centre;
        game_logic::items::Item::Colour colour;
        float radius{INITIAL_RADIUS};
        int phase{0};
        int t{0};
    public:
        HandleBomb(game_logic::items::Coord const& bombLocation, game_logic::items::Item::Colour const bombColour);
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        //No pointers to kill, so no need to override destructor.
    };
    namespace render{
        void renderBombHalo(game_logic::items::Coord const& coord, float radius){
            float centreX=coord.x+0.5f;
            float centreY=coord.y+0.5f;
            const SDL_Rect dstRect{
                static_cast<int>((centreX-radius)*CELL_LENGTH),//I believe cast is a round towards 0.
                static_cast<int>((centreY-radius)*CELL_LENGTH),
                static_cast<int>(2*radius*CELL_LENGTH),
                static_cast<int>(2*radius*CELL_LENGTH)};
            const SDL_Rect* srcRect//That is (variable pointer) to (const SDL_Rect).
                 = &BOMB_HALO_TEXTURE_REGION;
            SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
        }
        void renderBombHalo(game_logic::items::Coord const& coord, float radius, float opacity){
            int alpha=SDL_ALPHA_OPAQUE*opacity;
            if(alpha>SDL_ALPHA_OPAQUE)  alpha=SDL_ALPHA_OPAQUE;
            else if(alpha<0)            alpha=0;

            SDL_SetTextureAlphaMod(assets::store::tiles,alpha);
            renderBombHalo(coord,radius);
            SDL_SetTextureAlphaMod(assets::store::tiles,SDL_ALPHA_OPAQUE);
        }
    }
}
namespace game_logic::items{
    bool Bomb::breakSelf(Colour const cause){
        stillSealed=false;
        return false;
    }
    game_logic::effects::Effect* Bomb::detonateSelf(game_logic::items::Coord const& coord){//Returns effect responsible for detonating it.
        detonated=true;
        return new game_logic::effects::HandleBomb(coord,colour);
    }
}
namespace game_logic::effects{
    HandleBomb::HandleBomb(game_logic::items::Coord const& bombLocation, game_logic::items::Item::Colour const bombColour): centre(bombLocation), colour(bombColour) {}
    Effect::ExpiryState HandleBomb::update(int dt){
        t+=dt;
        if(phase==0){//If on the initial implosion.
            if(t>TOTAL_TIME_IMPLODE){//If have finished imploding, move onto explosion.
                t-=TOTAL_TIME_IMPLODE;
                phase=1;
            }
            else{//Otherwise calculate radius while imploding and return that are busy.
                radius=INITIAL_RADIUS-RADIAL_VELOCITY_IMPLODE*t;
                return ExpiryState::BUSY;
            }
        }
        if(phase<4){//If on explosion.
            radius=RADIAL_VELOCITY_IMPLODE+RADIAL_VELOCITY_EXPLODE*t;//Calulate radius.
            if(phase==1 && radius>=TRIGGER_RADIUS){//If have not yet attempted to destroyed the horizontally/vertically adjacent items, and radius is now big enough to do so.
                //Then go through the 4 directions, destroying items if can go in that direction and find a non-null breakable item.
                Direction direction{Direction::UP};
                for(int i=0; i<4; (++i, direction=direction.anticlockwise())){
                    if(centre<direction){
                        Coord const checking{centre+direction};
                        Item* const item=game_logic::game::grid->peek(checking);
                        if(item!=nullptr && item->state().isBreakable()){
                            game_logic::game::breakBreakableItemInGrid(item,colour,checking);
                        }
                    }
                }
                phase=2;//And record that have done this breaking.
            }
            if(phase==2 && radius>=TRIGGER_RADIUS*std::sqrt(2)){//If have not yet attempted to destroyed the diagonally adjacent items, and radius is now big enough to do so.
                //Then go through the 4 diagonal directions (by going one of the axis aligned directions then again the direction anticlockwise from it), destroying items if can go in that direction and find a non-null breakable item.
                Direction direction{Direction::UP};
                for(int i=0; i<4; (++i, direction=direction.anticlockwise())){
                    Coord checking{centre};
                    if(checking<direction){
                        checking+=direction;
                        if(checking<direction.anticlockwise()){
                            checking+=direction.anticlockwise();
                            Item* const item=game_logic::game::grid->peek(checking);
                            if(item!=nullptr && item->state().isBreakable()){
                                game_logic::game::breakBreakableItemInGrid(item,colour,checking);
                            }
                        }
                    }
                }
                phase=3;//And record that have done this breaking.
            }
            if(phase==3 && t>TOTAL_TIME_EXPLODE){//If have done all the breaking and have reached target explosion radius, then move onto fading.
                t-=TOTAL_TIME_EXPLODE;
                radius=TARGET_RADIUS_EXPLODE;
                phase=4;
            }
            else{//Otherwise say that are still busy.
                return ExpiryState::BUSY;
            }
        }
        /*phase should be 4 if reached here.*/
            if(t>FADE_TIME){//If done fading, say that are done.
                return ExpiryState::DONE;
            }
            else{//Otherwise say that are idle - still doing stuff, but don't need to wait for this.
                return ExpiryState::IDLE;
            }
    }
    void HandleBomb::draw(int dt) {
        if(phase<4){//If not fading, draw the explosion at the required radius.
            render::renderBombHalo(centre,radius);
        }
        else{//Otherwise draw it at the correct faded-ness.
            render::renderBombHalo(centre,TARGET_RADIUS_EXPLODE,1.f-(t/static_cast<float>(FADE_TIME)));
        }
    }
}