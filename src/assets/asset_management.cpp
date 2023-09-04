#include "asset_management.h"
#include "asset_store.h"
#include "window_admin.h"

#include <string>
#include <iostream>

namespace assets{
    namespace store::textures{
        SDL_Texture* tiles=nullptr;
    }
    
    namespace management{
        using namespace assets::store::textures;
        
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
            if(texture==nullptr){
                std::cout << "Couldn't load texture from '" << fileName << "'. Error: '" << IMG_GetError() << "'.\n";
                return false;
            }
            return true;
        }
        void freeTexture(SDL_Texture*& texture){
            if(texture!=nullptr){
                SDL_DestroyTexture(texture);
                texture=nullptr;
            }
        }
    }
}