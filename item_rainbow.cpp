#include "item_specials.h"

#include "effect.h"
#include "game.h"
#include "constants.h"
#include "render.h"

#include <vector>
#include <iterator>

using game_logic::items::Direction;

namespace game_logic::effects{
    //Effect to make rainbow halo spin, then implode, and explode out where there are matching items, breaking those there, before fading.
    class HandleRainbow:public Effect{
    protected:
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
    protected:
        Coord const source;
        int phase{0};
        int t{0};
        double angleAtEndOfPreviousPhase{0};
        std::vector<Coord> breakSites{};
    protected:
        HandleRainbow(game_logic::items::Coord const& rainbowLocation);
        virtual void goThroughBreakingItemsInGrid()=0;
        virtual void drawSingleHalo(float const radius, double const angle)=0;//Draw the halo at the source (virtual as how determine colour depends upon subclass).
        virtual void drawExplodingHalos(float const radius, double const angle)=0;//Draw the halo at break sites (virtual as how determine colour (and how iterate to line up the Colour with the Coord) depends upon subclass).
        virtual void drawFadingHalos(float const radius, double const angle, float const opacity)=0;//Draw the halo at break sites with opacity (virtual as how determine colour (and how iterate to line up the Colour with the Coord) depends upon subclass).
    public:
        static HandleRainbow* newHandleRainbow(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val);//Use this to automatically get object of the correct subclass.
        ExpiryState update(int dt) override;
        void draw(int dt) override;
        //Note to self: there is no need to clear breakSites in the destructor, as breakSites is "destructor-ed" when the owning object is. Would only have to deal with the vector if it contained pointers, as would have to delete those.
    };

    class HandleRainbowTargetSingle:public HandleRainbow{//For when set off a rainbow on a single colour.
    private:
        game_logic::items::Item::Colour targetColour;
    private:
        void goThroughBreakingItemsInGrid() override;
        void drawSingleHalo(float const radius, double const angle) override;
        void drawExplodingHalos(float const radius, double const angle) override;
        void drawFadingHalos(float const radius, double const angle, float const opacity) override;
    private://Constructor is private, and instead called from newHandleRainbow, which automatically dispenses an object of the correct HandleRainbow subclass.
        HandleRainbowTargetSingle(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val);
        friend HandleRainbow* HandleRainbow::newHandleRainbow(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val);
    };

    class HandleRainbowTargetAllColours:public HandleRainbow{//For when set off a pair of rainbowson each other.
    private:
        std::vector<game_logic::items::Item::Colour> breakSiteColours{};
    private:
        void goThroughBreakingItemsInGrid() override;
        void drawSingleHalo(float const radius, double const angle) override;
        void drawExplodingHalos(float const radius, double const angle) override;
        void drawFadingHalos(float const radius, double const angle, float const opacity) override;
    private:
        HandleRainbowTargetAllColours(game_logic::items::Coord const& rainbowLocation);
        friend HandleRainbow* HandleRainbow::newHandleRainbow(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val);
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
        return game_logic::effects::HandleRainbow::newHandleRainbow(coord,target);
    }
}
namespace game_logic::effects{
    HandleRainbow* HandleRainbow::newHandleRainbow(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val){
        if(targetColour_val==game_logic::items::Item::RAINBOW)
            return new HandleRainbowTargetAllColours(rainbowLocation);
        else
            return new HandleRainbowTargetSingle(rainbowLocation,targetColour_val);
    }

    HandleRainbow::HandleRainbow(game_logic::items::Coord const& rainbowLocation):source(rainbowLocation) {}
    Effect::ExpiryState HandleRainbow::update(int dt){
        t+=dt;
        while(phase<4){//If have spent enough time in one phase, move onto next (in while loop in case slow frame rate etc. means pass multiple phases in 1 frame).
            if(t>TOTAL_TIME_ARRAY[phase]){
                t-=TOTAL_TIME_ARRAY[phase];
                angleAtEndOfPreviousPhase+=angularDisplacement(phase,TOTAL_TIME_ARRAY[phase]);
                ++phase;
                if(phase==2){//If the phase change was to the one where the halos explode out elsewhere, then go through the grid breaking the relevant items.
                    goThroughBreakingItemsInGrid();
                }
            }
            else{
                return phase==3 ? ExpiryState::IDLE : ExpiryState::BUSY;//Phase 3 is just aesthetics, so are IDLE. The other phases do stuff, so are BUSY.
            }
        }
        return ExpiryState::DONE;
    }
    void HandleRainbow::draw(int dt) {
        double const angle=angleAtEndOfPreviousPhase+angularDisplacement(phase,t);

        switch(phase){
            case 0://Simply draw the halo at original radius at source.
                drawSingleHalo(RADIUS_INITIAL, angle);
                break;
            case 1://Draw the halo at source, but imploding.
                drawSingleHalo(RADIUS_INITIAL+t*RADIAL_VELOCITY_IMPLODE, angle);
                break;
            case 2://Draw the halos at the break sites, exploding.
                drawExplodingHalos(RADIUS_DISAPPEAR+t*RADIAL_VELOCITY_EXPLODE,angle);
                break;
            case 3:default://Draw the halos at the break sites, fading.
                drawFadingHalos(RADIUS_FADING,angle,1.f-(t/static_cast<float>(TOTAL_TIME_FADE)));
                break;
        }
    }

    HandleRainbowTargetSingle::HandleRainbowTargetSingle(game_logic::items::Coord const& rainbowLocation, game_logic::items::Item::Colour const targetColour_val):HandleRainbow(rainbowLocation), targetColour(targetColour_val){}
    void HandleRainbowTargetSingle::goThroughBreakingItemsInGrid(){
        //Go through the grid, breaking and recording the coordinates of any (non-null) items that are breakable and the target colour.
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
    void HandleRainbowTargetSingle::drawSingleHalo(float const radius, double const angle){
        render::renderRainbowHalo(source, radius, angle, targetColour);
    }
    void HandleRainbowTargetSingle::drawExplodingHalos(float const radius, double const angle){
        for(Coord coord:breakSites){
            render::renderRainbowHalo(coord, radius, angle, targetColour);
        }
    }
    void HandleRainbowTargetSingle::drawFadingHalos(float const radius, double const angle, float const opacity){
        for(Coord coord:breakSites){
            render::renderRainbowHalo(coord, radius, angle, targetColour, opacity);
        }
    }

    HandleRainbowTargetAllColours::HandleRainbowTargetAllColours(game_logic::items::Coord const& rainbowLocation):HandleRainbow(rainbowLocation){}
    void HandleRainbowTargetAllColours::goThroughBreakingItemsInGrid(){
        //Go through the grid, breaking and recording the coordinates and colour of any (non-null) items that are breakable and not rainbow coloured.
        for(Coord coord{0,0}; coord.x<util::constants::CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<util::constants::CELL_COUNT_VERTICAL; ++coord.y){
                Item* const item{game_logic::game::grid->peek(coord)};
                if(item!=nullptr && item->colour!=game_logic::items::Item::RAINBOW && item->state().isBreakable()){
                    breakSites.push_back(coord);
                    breakSiteColours.push_back(item->colour);
                    game_logic::game::breakBreakableItemInGrid(item,game_logic::items::Item::RAINBOW,coord);
                }
            }
        }
    }

    void HandleRainbowTargetAllColours::drawSingleHalo(float const radius, double const angle){
        render::renderRainbowHalo(source, radius, angle, game_logic::items::Item::RAINBOW);
    }
    void HandleRainbowTargetAllColours::drawExplodingHalos(float const radius, double const angle){
        auto iterateCoord=breakSites.begin();
        auto iterateColour=breakSiteColours.begin();
        for(;iterateCoord!=breakSites.end();(++iterateCoord,++iterateColour)){
            render::renderRainbowHalo(*iterateCoord, radius, angle, *iterateColour);
        }
    }
    void HandleRainbowTargetAllColours::drawFadingHalos(float const radius, double const angle, float const opacity){
        auto iterateCoord=breakSites.begin();
        auto iterateColour=breakSiteColours.begin();
        for(;iterateCoord!=breakSites.end();(++iterateCoord,++iterateColour)){
            render::renderRainbowHalo(*iterateCoord, radius, angle, *iterateColour, opacity);
        }
    }
}