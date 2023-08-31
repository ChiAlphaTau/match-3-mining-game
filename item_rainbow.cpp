#include "item_specials.h"

#include "effect.h"
#include "game.h"
#include "constants.h"
#include "render.h"

#include <vector>

using game_logic::items::Direction;

namespace game_logic::effects{
    //Effect to make rainbow halo spin, then implode, and explode out where there are matching items, breaking those there, before fading.
    class HandleRainbowSingleTarget:public Effect{
    private:
        const float RADIUS_INITIAL{0.5f};
        const float RADIUS_DISAPPEAR{0.f};
        const float RADIUS_FADING{1.f};

        const int TOTAL_TIME_JUST_SPIN{250};

        const int TOTAL_TIME_IMPLODE{750};
        const float RADIAL_VELOCITY_IMPLODE{(RADIUS_DISAPPEAR-RADIUS_INITIAL)/TOTAL_TIME_IMPLODE};

        const int TOTAL_TIME_EXPLODE{500};
        const float RADIAL_VELOCITY_EXPLODE{(RADIUS_FADING-RADIUS_DISAPPEAR)/TOTAL_TIME_EXPLODE};

        const int TOTAL_TIME_FADE{1000};

        const int TOTAL_TIME_ARRAY[4]{TOTAL_TIME_JUST_SPIN,TOTAL_TIME_IMPLODE,TOTAL_TIME_EXPLODE,TOTAL_TIME_FADE};//IDEA: Maybe change things so don't store these times twice.

        double angularDisplacement(int phase, int t){//Idea is that rotation angle is angle at end of previous phase, plus the extra rotation that happened during this phase.
            switch(phase){
                case 0:
                    return t*-360e-3;
                case 1:
                    return t*-360e-3*3;
                case 2:
                    return t*+360e-3*3;
                default:
                    return t*+360e-3;
            }
        }
    private:
        Coord const source;
        game_logic::items::Item::Colour targetColour;
        int phase{0};
        int t{0};
        double angleAtEndOfPreviousPhase{0};
        std::vector<Coord> breakSites{};
    public:
        HandleRainbowSingleTarget(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val);
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        //Note to self: there is no need to clear breakSites in the destructor, as breakSites is "destructor-ed" when the owning object is. Would only have to deal with the vector if it contained pointers, as would have to delete those.
    };
}
namespace game_logic::items{
    bool Rainbow::breakSelf(Colour const cause){
        stillSealed=false;
        target=cause;
        return false;
    }
    game_logic::effects::Effect* Rainbow::detonateSelf(game_logic::items::Coord const& coord){//Returns effect responsible for detonating it.
        detonated=true;
        return new game_logic::effects::HandleRainbowSingleTarget(coord,target);
    }
}
namespace game_logic::effects{
    HandleRainbowSingleTarget::HandleRainbowSingleTarget(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val):source(rainbowLocation), targetColour(targetColour_val){}
    Effect::ExpiryState HandleRainbowSingleTarget::update(int dt){
        t+=dt;
        while(phase<4){//If have spent enough time in one phase, move onto next (in while loop in case slow frame rate etc. means pass multiple phases in 1 frame).
            if(t>TOTAL_TIME_ARRAY[phase]){
                t-=TOTAL_TIME_ARRAY[phase];
                angleAtEndOfPreviousPhase+=angularDisplacement(phase,TOTAL_TIME_ARRAY[phase]);
                ++phase;
                if(phase==2){//If the phase change was to the one where the halos explode out elsewhere, then go through the grid, breaking and recording the coordinates of any (non-null) items that are breakable and the target colour.
                    for(Coord coord{0,0}; coord.x<util::constants::CELL_COUNT_HORIZONTAL; ++coord.x){
                        for(coord.y=0; coord.y<util::constants::CELL_COUNT_VERTICAL; ++coord.y){
                            Item* const item{game_logic::game::grid->peek(coord)};
                            if(item!=nullptr && item->colour==targetColour && item->state().isBreakable()){
                                breakSites.push_back(coord);
                                game_logic::game::breakBreakableItemInGrid(item,targetColour,coord);
                            }
                        }
                    }
                }
            }
            else{
                return phase==3 ? ExpiryState::IDLE : ExpiryState::BUSY;//Phase 3 is just aesthetics, so are IDLE. The other phases do stuff, so are BUSY.
            }
        }
        return ExpiryState::DONE;
    }
    void HandleRainbowSingleTarget::draw(int dt) {
        Uint8 const r=util::constants::colours_rgb::R[targetColour];
        Uint8 const g=util::constants::colours_rgb::G[targetColour];
        Uint8 const b=util::constants::colours_rgb::B[targetColour];
        double const angle=angleAtEndOfPreviousPhase+angularDisplacement(phase,t);

        switch(phase){
            case 0://Simply draw the halo at original radius at source.
                render::renderRainbowHalo(source, RADIUS_INITIAL, angle, r, g, b);
                break;
            case 1://Draw the halo at source, but imploding.
                render::renderRainbowHalo(source, RADIUS_INITIAL+t*RADIAL_VELOCITY_IMPLODE, angle, r, g, b);
                break;
            case 2://Draw the halos at the break sites, exploding.
                {//The {} is to wrap radius in a scope, otherwise it would be visible in the {case 3:default:} branch but not be initialised (see {https://stackoverflow.com/a/5685578}).
                    float const radius{RADIUS_DISAPPEAR+t*RADIAL_VELOCITY_EXPLODE};
                    for(Coord coord:breakSites){
                        render::renderRainbowHalo(coord, radius, angle, r, g, b);
                    }
                }
                break;
            case 3:default://Draw the halos at the break sites, fading.
                for(Coord coord:breakSites){
                    render::renderRainbowHalo(coord, RADIUS_FADING, angle, r, g, b, 1.f-(t/static_cast<float>(TOTAL_TIME_FADE)));
                }
                break;
        }
    }
}