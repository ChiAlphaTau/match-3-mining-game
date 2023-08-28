#include "falling.h"

#include "game.h"

using game_logic::items::Item;

namespace game_logic::effects{
    Fall::Fall(game_logic::items::Coord const& bottomCoordVal, const int itemCountVal, std::vector<Item*> const& fallingItems):
        bottomCoord{bottomCoordVal}, itemCount{itemCountVal}, items{fallingItems}{
            updateS();
        }
    void Fall::updateS(){
        s = a*t*t/2.f - itemCount;
    }
    Effect::ExpiryState Fall::update(int dt){
        t+=dt;
        updateS();
        if(s<0){//If fall is not yet complete, say that are busy.
            return Effect::BUSY;
        }
        else{//Otherwise are done - place items at final destination, clear items, and return that are done.
            game_logic::items::Coord head{bottomCoord};
            for(Item* item:items){
                game_logic::game::grid -> give(head,item);
                //Possibly should null items entry, but vector is about to be cleared so ~no need.
                head+=game_logic::items::Direction::UP;
            }
            items.clear();
            return Effect::DONE;
        }
    }
    void Fall::draw(int dt){
        float dy=s;
        for(Item* item:items){
            if(bottomCoord.y+dy<=-1)  break;//Only bother drawing items that are on the screen.
            item->draw(dt,bottomCoord,0,dy);
            dy-=1;
        }
    }
    Fall::~Fall(){
        for(Item* item:items){
            if(item!=NULL){//Shouldn't contain NULL entries, but just in case.
                delete item;
                //Possibly should null items entry, but vector is about to be cleared so ~no need.
            }
        }
        items.clear();
    }
}