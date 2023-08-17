#include "asset_management.h"
#include "asset_store.h"
#include "window_admin.h"
#include "constants.h"

#include <string>
#include <iostream>

using std::cout;

namespace assets{
    namespace store{
        SDL_Texture* tiles=NULL;
    }
    
    namespace management{
        using namespace assets::store;
        
        bool loadTexture(const std::string& fileName, SDL_Texture*& texture);
        void freeTexture(SDL_Texture*& texture);
        bool load(){
            bool success{true};
            
            success&=loadTexture("images/tiles.png",tiles);
            
            return success;
        }
        void destroy(){
            freeTexture(tiles);
        }
        
        bool loadTexture(const std::string& fileName, SDL_Texture*& texture){
            texture = IMG_LoadTexture(program::renderer,fileName.c_str());
            if(texture==NULL){
                cout << "Couldn't load texture from '" << fileName << "'. Error: '" << IMG_GetError() << "'." << NEWLINE;
                return false;
            }
            return true;
        }
        void freeTexture(SDL_Texture*& texture){
            if(texture!=NULL){
                SDL_DestroyTexture(texture);
                texture=NULL;
            }
        }
    }
}