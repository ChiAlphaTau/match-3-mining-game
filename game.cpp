#include "game.h"

#include "window_admin.h"
#include "constants.h"
#include "coordinate.h"
#include "swap.h"
#include "default_item_rendering.h"

#include <SDL2/SDL.h>

#include <vector>
#include <iterator>

using namespace util::constants;
using game_logic::effects::Effect;

namespace game_logic::game{
    Grid* grid;//TODO: Why was I getting double deletes or something when I tried using a Grid intead of Grid*?
    std::vector<Effect*> effectList{};
    std::vector<Effect*> pendingEffects{};

    bool effectLoop(const int dt);
    void shuffleAboutEffects();

    bool setup(){
        grid=new Grid();

        pushEffect(new game_logic::effects::SwapFailed(Coord{0,1}, Coord{1,1}));
        pushEffect(new game_logic::effects::SwapFailed(Coord{7,8}, Coord{8,9}));

        return true;
    }
    void close(){
        shuffleAboutEffects();//Makes it so effects have been added as they have expected, and pendingEffects is now empty.
        for(Effect* effect:effectList){
            effect->cutShort();
            delete effect;
        }
        effectList.clear();

        delete grid;
    }
    void handleEvent(const SDL_Event& event, const int dt){
        //TODO.
    }
    void update(const int dt){
        effectLoop(dt);
        shuffleAboutEffects();
        //TODO:state change based on effectLoop return value.
    }
    void draw(const int dt){
        //Clear screen, draw items in grid, then get effects to draw themselves.
        SDL_RenderClear(program::renderer);
        Item* item;
        for(int x=0; x<CELL_COUNT_HORIZONTAL; ++x){
            for(int y=0; y<CELL_COUNT_VERTICAL; ++y){
                item=grid->peek(Coord{x,y});
                if(item==NULL)continue;
                game_logic::items::render::renderItemDefaultly(item);
            }
        }
        for(Effect* effect:effectList)  effect->draw(dt);
    }
    void pushEffect(game_logic::effects::Effect* effect){
        pendingEffects.push_back(effect);
    }
    bool effectLoop(const int dt){
        bool notStillBusy{true};
        for(auto it=effectList.begin(); it!=effectList.end();){
            switch((*it)->update(dt)){
                case game_logic::effects::Effect::DONE:
                    delete *it;
                    it=effectList.erase(it);
                    break;
                case game_logic::effects::Effect::BUSY:
                    notStillBusy=false;
                    //No break is purposeful.
                case game_logic::effects::Effect::IDLE:
                    (*it)->draw(dt);
                    ++it;
            }
        }
        return notStillBusy;
    }
    //**If there are effects waiting to be added, then: add them in priority order (priority ties broken on first come, first served basis), and then clear that list of waiting effects.
    void shuffleAboutEffects(){
        if(!pendingEffects.empty()){
            for(Effect* effectToAdd : pendingEffects){
                for(auto it=effectList.begin(); it!=effectList.end(); ++it){
                    if((*it) -> priority() > effectToAdd->priority()){
                        effectList.insert(it,effectToAdd);
                        //Break out of the inner for loop, AND skip adding the effect to the end.
                        //This is (hopefully) equivalent to continue, but making it apply to the outer for instead of inner for.
                        //I.e. like using {continue label} from Java.
                        goto endOfLoopOverEffects;
                    }
                }
                effectList.push_back(effectToAdd);
                endOfLoopOverEffects:;
            }
            pendingEffects.clear();
        }
    }
}