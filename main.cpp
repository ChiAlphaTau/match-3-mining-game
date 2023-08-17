#include "window_admin.h"
#include "constants.h"
#include "asset_management.h"
#include "asset_store.h" //TODO:remove this once drawing is not done here.

#include <iostream>

using std::cout;
using namespace util::constants;//TODO:remove this once drawing is not done here.

int main(int argc, char **argv){
    if(!Program::setupWindow()){
        cout << "Program setup failed; aborting." << NEWLINE;
        return 1;
    }

    if(!assets::management::load()){
        cout << "Asset loading failed; aborting." << NEWLINE;
        return 2;
    }

    bool stillLooping{true};
    SDL_Event event;
    while(stillLooping){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                stillLooping=false;
            }
        }
        if(stillLooping){
            SDL_RenderClear(Program::renderer);
            SDL_Rect dstRect{0,0,CELL_LENGTH,CELL_LENGTH};
            for(int x=0; x<CELL_COUNT_HORIZONTAL; (++x,dstRect.x+=CELL_LENGTH)){
                dstRect.y=0;
                for(int y=0; y<CELL_COUNT_VERTICAL; (++y,dstRect.y+=CELL_LENGTH)){
                    SDL_RenderCopy(Program::renderer,assets::store::tiles,&GEM_TEXTURE_REGIONS[(x+y)%NUMBER_OF_COLOURS],&dstRect);
                }
            }
            Program::refreshScreen();
        }
    }

    assets::management::destroy();
    Program::close();
}
