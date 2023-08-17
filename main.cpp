#include "window_admin.h"
#include "constants.h"
#include "asset_management.h"
#include "game.h"

#include <iostream>

using std::cout;

int main(int argc, char **argv){
    if(!program::setupWindow()){
        cout << "Program setup failed; aborting." << NEWLINE;
        return 1;
    }

    if(!assets::management::load()){
        cout << "Asset loading failed; aborting." << NEWLINE;
        return 2;
    }

    if(!game_logic::game::setup()){
        cout << "Couldn't setup game; aborting." << NEWLINE;
        return 3;
    }

    bool stillLooping{true};
    auto previousTime = SDL_GetTicks();
    decltype(previousTime) currentTime{0};
    int dt=0;
    SDL_Event event;
    while(stillLooping){
        currentTime=SDL_GetTicks();
        dt=currentTime-previousTime;
        previousTime=currentTime;

        while(SDL_PollEvent(&event)){
            game_logic::game::handleEvent(event,dt);
            if(event.type==SDL_QUIT){
                stillLooping=false;
            }
        }

        if(stillLooping){
            game_logic::game::update(dt);
            game_logic::game::draw(dt);
            program::refreshScreen();
        }
    }

    game_logic::game::close();
    assets::management::destroy();
    program::close();
}
