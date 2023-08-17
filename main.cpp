#include "window_admin.h"
#include "constants.h"

#include <iostream>

using std::cout;

int main(int argc, char **argv){
    if(!Program::setupWindow()){
        cout << "Program setup failed; aborting." << NEWLINE;
        return 1;
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
            Program::refreshScreen();
        }
    }
    Program::close();
}
