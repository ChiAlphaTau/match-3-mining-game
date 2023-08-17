#include "game.h"

#include "window_admin.h"
#include "constants.h"
#include "asset_store.h"

#include <SDL2/SDL.h>

using namespace util::constants;

namespace game_logic::game{
    bool setup(){
        //TODO.
        return true;
    }
    void close(){
        //TODO.
    }
    void handleEvent(const SDL_Event& event, const int dt){
        //TODO.
    }
    void update(const int dt){
        //TODO.
    }
    void draw(const int dt){
        SDL_RenderClear(program::renderer);
        //TODO: change to proper grid thing.
        SDL_Rect dstRect{0,0,CELL_LENGTH,CELL_LENGTH};
        for(int x=0; x<CELL_COUNT_HORIZONTAL; (++x,dstRect.x+=CELL_LENGTH)){
            dstRect.y=0;
            for(int y=0; y<CELL_COUNT_VERTICAL; (++y,dstRect.y+=CELL_LENGTH)){
                SDL_RenderCopy(program::renderer,assets::store::tiles,&GEM_TEXTURE_REGIONS[(x+y)%NUMBER_OF_COLOURS],&dstRect);
            }
        }
    }
}