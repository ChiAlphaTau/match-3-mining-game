#include "game.h"

#include "window_admin.h"
#include "constants.h"
#include "asset_store.h"
#include "coordinate.h"

#include <SDL2/SDL.h>

using namespace util::constants;

namespace game_logic::game{
    Grid* grid;//TODO: Why was I getting double deletes or something when I tried using a Grid intead of Grid*?
    bool setup(){
        grid=new Grid();
        return true;
    }
    void close(){
        delete grid;
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
        SDL_Rect dstRect{0,0,CELL_LENGTH,CELL_LENGTH};
        const SDL_Rect* srcRect;//That is (variable pointer) to (const SDL_Rect).
        Item* item;
        for(int x=0; x<CELL_COUNT_HORIZONTAL; ++x){
            for(int y=0; y<CELL_COUNT_VERTICAL; ++y){
                item=grid->peek(Coord{x,y});
                if(item==NULL)continue;
                srcRect=&GEM_TEXTURE_REGIONS[item->colour];
                dstRect.x = item->x*CELL_LENGTH;
                dstRect.y = item->y*CELL_LENGTH;
                SDL_RenderCopy(program::renderer,assets::store::tiles,srcRect,&dstRect);
            }
        }
    }
}