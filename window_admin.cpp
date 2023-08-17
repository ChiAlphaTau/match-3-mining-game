#include "window_admin.h"

#include "constants.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace util::constants;
using std::cout;

namespace Program{
    SDL_Window* window=NULL;
    SDL_Renderer* renderer=NULL;

    bool setupWindow(){
        if(SDL_Init(SDL_INIT_VIDEO)<0){
            cout << "Could not load SDL. Error:'" << SDL_GetError() << NEWLINE;
            return false;
        }

        window=SDL_CreateWindow("Match 3",640,0,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
        if(window==NULL){
            cout << "Could not load window. Error:'" << SDL_GetError() << NEWLINE;
            return false;
        }

        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(renderer==NULL){
            cout << "Could not create renderer. Error:'" << SDL_GetError() << NEWLINE;
            return false;
        }
        SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,SDL_ALPHA_OPAQUE);
        
        const int SDL_IMG_FLAGS= IMG_INIT_PNG;
        if((IMG_Init(SDL_IMG_FLAGS) & SDL_IMG_FLAGS) != SDL_IMG_FLAGS){
            cout << "Could not enable SDL_IMG. Error:'" << IMG_GetError() << NEWLINE;
        }

        return true;
    }
    
    void close(){
        SDL_DestroyRenderer(renderer);renderer=NULL;
        SDL_DestroyWindow(window);window=NULL;
        
        IMG_Quit();
        SDL_Quit();
    }
    
    void refreshScreen(){
        SDL_RenderPresent(renderer);
    }
}