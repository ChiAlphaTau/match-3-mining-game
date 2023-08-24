#include "game.h"

#include "window_admin.h"
#include "constants.h"
#include "coordinate.h"
#include "swap.h"
#include "item_vanilla.h"
#include "board_logic.h"

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
    BoardState boardState{AWAITING_INPUT};

    bool effectLoop(const int dt);
    void shuffleAboutEffects();
    Coord getCoordOfScreenPosition(int x,int y);
    void drawSelectionIndicator();
    void moveSelectedFromMouseUp(SDL_MouseButtonEvent const& event);
    void moveSelectedFromKeyUp(SDL_KeyboardEvent const& event);
    void makeSwap(Coord const origin, game_logic::items::Direction const direction);
    bool isSwapLegalInSingleDirection(Coord const origin, game_logic::items::Direction direction);
    bool isSwapLegal(Coord const origin, game_logic::items::Direction const direction);

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
        /*
         * WARNING: I can imagine weird stuff happening if an event is added during the shuffleAboutEffects the update call after notStillBusy==true.
         * This is because it may want to say that it is still busy in the previous, but finds it in the new state.
         * You might purposefully add an Effect to be busy in the next state, so adding an Effect that will be busy isn't inherently an error.
         * Just if weird stuff is happening, this may be the cause.
         */
        shuffleAboutEffects();
        bool notStillBusy = effectLoop(dt);
        if(notStillBusy){
            switch(boardState){
                case AWAITING_INPUT:
                    //Do nothing - carry on waiting.
                    break;
                case TEMPORARILY_NOT_ACCEPTING_INPUT:
                    boardState=AWAITING_INPUT;//Donw with whatever graphical thing you were dowing, and so are accepting input once more.
                    break;
                case RESOLVING:
                    if(board_update::tryToMakeFallsHappen()){
                        boardState=MOVING;
                    }
                    else{
                        boardState=AWAITING_INPUT;
                    }
                    break;
                case MOVING:
                    if(board_update::tryToFindMatches()){
                        boardState=RESOLVING;
                    }
                    else{
                        boardState=AWAITING_INPUT;
                    }
                    break;
            }
        }
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
    BoardState getBoardState(){
        return boardState;
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
        if(boardState==AWAITING_INPUT){
            if(selected){
                game_logic::items::Direction movementDirection = clickedCoord-selectionPosition;
                if(movementDirection!=game_logic::items::Direction::NONE){
                    if(grid->peek(selectionPosition)!=NULL && grid->peek(clickedCoord)!=NULL){
                        makeSwap(selectionPosition,movementDirection);
                        selectionPosition=clickedCoord;
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
        else{
            selectionPosition=clickedCoord;
        }
    }
    void moveSelectedFromKeyUp(SDL_KeyboardEvent const& event){
        using game_logic::items::Direction;
        Direction arrowDirection{Direction::NONE};
        switch(event.keysym.sym){
            case SDLK_RETURN: case SDLK_KP_ENTER:
                if(boardState==AWAITING_INPUT)  selected=!selected;
                break;
            case SDLK_UP:case SDLK_w:
                arrowDirection=Direction::UP;
                break;
            case SDLK_DOWN:case SDLK_s:
                arrowDirection=Direction::DOWN;
                break;
            case SDLK_LEFT:case SDLK_a:
                arrowDirection=Direction::LEFT;
                break;
            case SDLK_RIGHT:case SDLK_d:
                arrowDirection=Direction::RIGHT;
                break;
        }

        if(arrowDirection!=Direction::NONE && selectionPosition<arrowDirection){
            if(selected){
                makeSwap(selectionPosition,arrowDirection);
            }
            selectionPosition+=arrowDirection;
        }
    }
    void makeSwap(Coord const origin, game_logic::items::Direction const direction){
        if(direction!=game_logic::items::Direction::NONE){//Shouldn't be NONE, but if pass 2 copies of the same coord, will have 2 copies of same Item*, thinking they are different, and presumably a double free will occur eventually.
            if(isSwapLegal(origin,direction)){
                pushEffect(new game_logic::effects::Swap<true>(origin,origin+direction));
                boardState=MOVING;
            }
            else{
                pushEffect(new game_logic::effects::Swap<false>(origin,origin+direction));
                boardState=TEMPORARILY_NOT_ACCEPTING_INPUT;
            }
        }
        selected=false;
    }
    bool isSwapLegalInSingleDirection(Coord const origin, game_logic::items::Direction direction){
        using board_update::moveInThatDirectionAndReportWhetherItIsTheSameColour;
        Item const* item=grid->peek(origin);//Origin is where the item came from.
        if(item==NULL)return false;
        game_logic::items::Item::Colour const colour = item->colour;
        Coord const start=origin+direction;//STart is where item is going, so where match checks are from.
        Coord checking{start};

        //First try going forward 2.
        if(
            moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour) &&
            moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour)
        )   return true;

        //Next try going anticlockwise 1.
        direction=direction.anticlockwise();
        checking=start;
        if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour)){
            //Then check if can make a 2nd anticlockwise.
            if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour))
                return true;

            //If can't see if have a match 1 anticlockwise, 1 clockwise.
            else{
                checking=start;
                if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,-direction,colour))
                    {
                        return true;
                    }
            }
        }

        //Next try going clockwise 2.
        checking=start;
        direction=-direction;
        if(
            moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour) &&
            moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,direction,colour)
        )   return true;

        //Run out of stuff to check.
        return false;
    }
    bool isSwapLegal(Coord const origin, game_logic::items::Direction const direction){
        return
            isSwapLegalInSingleDirection(origin,direction) ||
            isSwapLegalInSingleDirection(origin+direction,-direction);
    }
}