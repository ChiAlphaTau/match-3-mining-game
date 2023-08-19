#include "game.h"

#include "window_admin.h"
#include "constants.h"
#include "coordinate.h"
#include "swap.h"
#include "item_vanilla.h"

#include <SDL2/SDL.h>

#include <vector>
#include <iterator>

using namespace util::constants;
using game_logic::effects::Effect;

namespace game_logic::game{
    Grid* grid;//TODO: Why was I getting double deletes or something when I tried using a Grid intead of Grid*?
    std::vector<Effect*> effectList{};
    std::vector<Effect*> pendingEffects{};
    Coord selectionPosition{0,0};
    bool selected{false};

    bool effectLoop(const int dt);
    void shuffleAboutEffects();
    Coord getCoordOfScreenPosition(int x,int y);
    void drawSelectionIndicator();
    void moveSelectedFromMouseUp(SDL_MouseButtonEvent const& event);
    void moveSelectedFromKeyUp(SDL_KeyboardEvent const& event);

    bool setup(){
        grid=new Grid();
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                grid->give(coord,new game_logic::items::ItemVanilla{items::colourFromInt((coord.x+(coord.y%2))%util::constants::NUMBER_OF_COLOURS)});
            }
        }

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
        if(event.type==SDL_MOUSEBUTTONUP && event.button.button==SDL_BUTTON_LEFT){
            moveSelectedFromMouseUp(event.button);
        }
        else if(event.type==SDL_KEYUP){
            moveSelectedFromKeyUp(event.key);
        }
    }
    void update(const int dt){
        effectLoop(dt);
        shuffleAboutEffects();
        //TODO:state change based on effectLoop return value.
    }
    void draw(const int dt){
        //Clear screen, draw items in grid, then get effects to draw themselves.
        SDL_SetRenderDrawColor(program::renderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);
        SDL_RenderClear(program::renderer);
        Item* item;
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                item=grid->peek(coord);
                if(item==NULL)continue;
                item->draw(dt,coord);
            }
        }
        for(Effect* effect:effectList)  effect->draw(dt);
        drawSelectionIndicator();
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
    Coord getCoordOfScreenPosition(int x,int y){
        return Coord{x/CELL_LENGTH,y/CELL_LENGTH};
    }
    void drawSelectionIndicator(){
        if(selected){
            SDL_SetRenderDrawColor(program::renderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
        }
        else{
            SDL_SetRenderDrawColor(program::renderer,0x77,0x77,0x77,SDL_ALPHA_OPAQUE);
        }
        SDL_Rect rect{
            selectionPosition.x*CELL_LENGTH,
            selectionPosition.y*CELL_LENGTH,
            CELL_LENGTH,CELL_LENGTH
        };
        SDL_RenderDrawRect(program::renderer,&rect);
    }
    void moveSelectedFromMouseUp(SDL_MouseButtonEvent const& event){
        Coord clickedCoord=getCoordOfScreenPosition(event.x,event.y);
        if(selected){
            if(selectionPosition.isAdjacentTo(clickedCoord)){
                if(grid->peek(selectionPosition)!=NULL && grid->peek(clickedCoord)!=NULL){
                    pushEffect(new game_logic::effects::SwapFailed(selectionPosition,clickedCoord));
                    selected=false;
                }
                else{
                    selectionPosition=clickedCoord; selected=true;
                }
            }
            else if(selectionPosition==clickedCoord){
                selected=false;
            }
            else{
                selectionPosition=clickedCoord; selected=true;
            }
        }
        else{
            selectionPosition=clickedCoord; selected=true;
        }
    }
    void moveSelectedFromKeyUp(SDL_KeyboardEvent const& event){
        if(event.keysym.sym==SDLK_RETURN || event.keysym.sym==SDLK_KP_ENTER){
                selected=!selected;
            }
    }
}