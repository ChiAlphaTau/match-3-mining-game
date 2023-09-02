#include "item_specials.h"

#include "effect.h"
#include "game.h"
#include "render.h"

using game_logic::items::Direction;

namespace game_logic::effects{
    //Effect to make star implode a bit, then explode and fade, shooting out lasers in the 4 directions (when explosion reaches peak) that destroy items as they go.
    class HandleStar:public Effect{
    private:
        const float INITIAL_RADIUS{0.5f};
        const float TARGET_RADIUS_IMPLODE{0.25f};
        const int TOTAL_TIME_IMPLODE{1000};
        const float RADIAL_VELOCITY_IMPLODE{(INITIAL_RADIUS-TARGET_RADIUS_IMPLODE)/TOTAL_TIME_IMPLODE};

        const float LASER_VELOCITY{8.f/1000.f};
        const float RADIAL_VELOCITY_EXPLODE{LASER_VELOCITY};
        const float TARGET_RADIUS_EXPLODE{1.f};
        const int TOTAL_TIME_EXPLODE{static_cast<int>((TARGET_RADIUS_EXPLODE-TARGET_RADIUS_IMPLODE)/RADIAL_VELOCITY_EXPLODE)};

        const int FADE_TIME{1000};

        const Direction LASER_DIRECTION[4]{Direction::UP,Direction::LEFT,Direction::DOWN,Direction::RIGHT};
        const double LASER_IMAGE_ROTATION_ANGLE[4]{0,-90*1,-90*2,-90*3};
        const int LASER_DISTANCE_SCALAR_X[4]{0,-1,0,+1};
        const int LASER_DISTANCE_SCALAR_Y[4]{-1,0,+1,0};
    private:
        Coord const centre;
        Coord laserEdge[4];
        game_logic::items::Item::Colour colour;
        int t{0};
        int phase{0};
        bool doneFading{false};
        float laserDistance{0};
        int nextItemToBreak{1};
    public:
        HandleStar(game_logic::items::Coord const& bombLocation, game_logic::items::Item::Colour const bombColour);
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        //No pointers to kill, so no need to override destructor.
    };
}
namespace game_logic::items{
    bool Star::breakSelf(Colour const cause){
        stillSealed=false;
        return false;
    }
    game_logic::effects::Effect* Star::detonateSelf(game_logic::items::Coord const& coord){//Returns effect responsible for detonating it.
        detonated=true;
        return new game_logic::effects::HandleStar(coord,colour);
    }
}
namespace game_logic::effects{
    HandleStar::HandleStar(game_logic::items::Coord const& starLocation, game_logic::items::Item::Colour const starColour): centre(starLocation),laserEdge{starLocation,starLocation,starLocation,starLocation}, colour(starColour) {}
    Effect::ExpiryState HandleStar::update(int dt) {
        t+=dt;
        if(phase==0){//If on the initial implosion.
            if(t>TOTAL_TIME_IMPLODE){//If have finished imploding, move onto explosion.
                t-=TOTAL_TIME_IMPLODE;
                phase=1;
            }
            else{//Otherwise return that are still busy (imploding).
                return ExpiryState::BUSY;
            }
        }
        if(phase==1){//If on the explosion.
            if(t>TOTAL_TIME_EXPLODE){//If have finished exploding, move to fading&shooting.
                t-=TOTAL_TIME_EXPLODE;
                phase=2;
            }
            else{//Otherwise return that are still busy (exploding).
                return ExpiryState::BUSY;
            }
        }
        /*Here must have phase=2 or 3.*/
            if(!doneFading){//If are still fading the explosion cloud.
                if(t>FADE_TIME){//If have done fading explosion cloud, record as such.
                    doneFading=true;
                }
            }
        if(phase==2){//If the lasers are still shooting outwards.
            laserDistance=1+t*LASER_VELOCITY;//Calculate how far from the centre of the old star they are.
            while(laserDistance>=nextItemToBreak){//If the lasers have reached a new item to attempt to break (this is in while loop in case slow framerate etc. means that one laser reaches multiple items in one frame).
                bool anyMoved{false};
                for(int i=0; i<4; ++i){//Go through trying to destroy the next item in that direction.
                    if(laserEdge[i]<LASER_DIRECTION[i]){//If can go in that direction.
                        anyMoved=true;
                        laserEdge[i]+=LASER_DIRECTION[i];
                        Item* const item=game_logic::game::grid->peek(laserEdge[i]);
                        if(item!=nullptr && item->state().isBreakable()){//And find something breakable, then break it.
                            game_logic::game::breakBreakableItemInGrid(item,colour,laserEdge[i]);
                        }
                    }
                }
                ++nextItemToBreak;
                //If couldn't move any of the laser Coords to a new valid coordinate, then are done shooting lasers.
                //This means that the change to phase 3 happens when all heads are off the screen; it doesn't check for tails.
                if(!anyMoved){
                    phase=3;
                    break;
                }
            }
        }
        if(phase==3){//If the lasers are done, then if explosion cloud is still fading then return that are idle, otherwise everything is done, so return as such.
            return doneFading?ExpiryState::DONE:ExpiryState::IDLE;
        }
        else{//Otherwise are still in the laser shooting phase: return that are still busy.
            return ExpiryState::BUSY;
        }
    }
    void HandleStar::draw(int dt) {
        if(phase==0){//If on implode, draw the star.
            render::renderStarHalo(centre,INITIAL_RADIUS-RADIAL_VELOCITY_IMPLODE*t);
        }
        else if(phase==1){//If on the explode, draw the star.
            render::renderStarHalo(centre,TARGET_RADIUS_IMPLODE+RADIAL_VELOCITY_EXPLODE*t);
        }
        else{//Otherwise shooting lasers/fading.
            if(!doneFading){//If fading isn't done, then draw the fading star.
                render::renderStarHalo(centre,TARGET_RADIUS_EXPLODE,1.f-(t/static_cast<float>(FADE_TIME)));
            }
            if(phase==2){//If lasers aren't done shooting, then draw them.
                for(int i=0; i<4; ++i){
                render::renderStarLaser(
                    centre.x+0.5f+LASER_DISTANCE_SCALAR_X[i]*laserDistance,
                    centre.y+0.5f+LASER_DISTANCE_SCALAR_Y[i]*laserDistance,
                    0.125f,
                    LASER_IMAGE_ROTATION_ANGLE[i]
                    );
                }
            }
        }
    }
}