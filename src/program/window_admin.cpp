#include "window_admin.h"

#include "constants_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace program::constants;
using std::cout;
#ifndef NEWLINE
#define NEWLINE '\n'
#endif

namespace program{
    SDL_Window* window=nullptr;
    SDL_Renderer* renderer=nullptr;

    bool setupWindow(){
        if(SDL_Init(SDL_INIT_VIDEO)<0){
            cout << "Could not load SDL. Error:'" << SDL_GetError() << NEWLINE;
            return false;
        }

        window=SDL_CreateWindow("Match 3",640,0,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
        if(window==nullptr){
            cout << "Could not load window. Error:'" << SDL_GetError() << NEWLINE;
            return false;
        }

        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(renderer==nullptr){
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
        SDL_DestroyRenderer(renderer);renderer=nullptr;
        SDL_DestroyWindow(window);window=nullptr;
        
        IMG_Quit();
        SDL_Quit();
    }
    
    void refreshScreen(){
        SDL_RenderPresent(renderer);
    }
}