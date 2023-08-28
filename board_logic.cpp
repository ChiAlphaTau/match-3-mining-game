#include "board_logic.h"

#include "game.h"
#include "coordinate.h"
#include "constants.h"
#include "falling.h"
#include "item_vanilla.h"

#include <vector>

using game_logic::game::grid;
using game_logic::items::Coord;
using game_logic::items::Direction;
typedef game_logic::items::Item::Colour Colour;
using util::constants::CELL_COUNT_HORIZONTAL;
using util::constants::CELL_COUNT_VERTICAL;

namespace game_logic::game::board_update{
    bool tryToFindMatchesInDirection(bool const upInsteadOfRight, bool (&shouldGo)[CELL_COUNT_HORIZONTAL][CELL_COUNT_VERTICAL]){
        //I shall be commenting as if forward is vertical.
        Direction const forward  = upInsteadOfRight ? Direction::DOWN  : Direction::RIGHT;
        Direction const sideways = upInsteadOfRight ? Direction::RIGHT : Direction::DOWN;
        bool foundAny{false};

        Coord start{0,0};
        while(true){
            Coord checking{start};
            while(checking<forward){
                while(grid->peek(checking)==NULL){//Find the next non-null grid entry in this column. If no more exist, then continue outer for.
                    if(checking<forward)   checking+=forward;
                    else                    goto nextSidewaysLoopIteration;
                }
                Colour colour=grid->peek(checking)->colour;//Get the colour of the item.
                if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){//If the next in the column is the same colour.
                    if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){//Then also check the one after. If it is also the same colour, then we have a match.
                        //Record that found some.
                        foundAny=true;
                        //Mark the 3 found items on the grid.
                            shouldGo[checking.x][checking.y]=true;
                        Coord recent=checking+-forward;
                            shouldGo[recent.x][recent.y]=true;
                        recent+=-forward;
                            shouldGo[recent.x][recent.y]=true;
                        //If the match extends beyond, then mark those too.
                        while(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){
                            shouldGo[checking.x][checking.y]=true;
                        }
                    }
                }
            }
            
        nextSidewaysLoopIteration:
        if(start<sideways)  start+=sideways;
        else                break;
        }

        return foundAny;
        
    }
    void removeMatches( bool (&shouldGo)[CELL_COUNT_HORIZONTAL][CELL_COUNT_VERTICAL]){
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                if(shouldGo[coord.x][coord.y]){
                    grid->destroy(coord);
                }
            }
        }
    }

    int tryToMakeFallsHappenInColumn(int const x){
        int fall{0};//Used to determine how far to make the items fall by, and returned at the end to say how many items to spawn.
        std::vector<Item*> claimed{};
        bool hitTopWhichIsNotNull{false};
        Coord head{x,CELL_COUNT_VERTICAL-1};

        //Skip past the bottom non-null entries, returning 0 if there are no falls.
        while(grid->peek(head)!=NULL){
            if(head<Direction::UP)  head+=Direction::UP;
            else                    return 0;
        }

        //Then until run out of cells to check.
        while(true){
            //Record destination of bottom falling item.
            int const bottom{head.y};
            //Work out how many consecutive gaps there are, returning total fall if run out of cells to check (i.e. if top cell is empty).
            do{
                ++fall;
                if(head<Direction::UP)  head+=Direction::UP;
                else                    return fall;
            } while(grid->peek(head)==NULL);
            //Then gather together the consecutive (non-null) items.
            do{
                claimed.push_back(grid->claim(head));
                if(head<Direction::UP)  head+=Direction::UP;
                else                    {hitTopWhichIsNotNull=true; break;}
            } while(grid->peek(head)!=NULL);
            //Then create a fall out of those gathered items.
            pushEffect(new game_logic::effects::Fall(game_logic::items::Coord(x,bottom),fall,claimed));
            claimed.clear();
            if(hitTopWhichIsNotNull)    return fall;//If you aborted the previous loop due to running out of cells to check (i.e. top cell is occupied), then return fall.
        }
    }
    void spawnItems(int const x, int const count){
        game_logic::items::Coord bottomCoord{x,count-1};
        std::vector<Item*> spawned{};
        for(int i=0; i<count; ++i){
            spawned.push_back(new game_logic::items::ItemVanilla(game_logic::items::colourFromInt((i+x)%util::constants::NUMBER_OF_COLOURS)));//TODO:Make colour random.
        }
        pushEffect(new game_logic::effects::Fall(bottomCoord,count,spawned));
    }

    bool tryToFindMatches(){
        bool shouldGo[CELL_COUNT_HORIZONTAL][CELL_COUNT_VERTICAL];
        for(int i=0;i<CELL_COUNT_HORIZONTAL;++i){
            for(int j=0;j<CELL_COUNT_VERTICAL;++j){
                shouldGo[i][j]=false;
            }
        }

        bool foundAny=  tryToFindMatchesInDirection(true,shouldGo);
        foundAny|=      tryToFindMatchesInDirection(false,shouldGo);
        
        if(foundAny){
            removeMatches(shouldGo);
        }

        return foundAny;
    }
    bool tryToMakeFallsHappen(){
        bool anyFalls{false};
        for(int x=0; x<CELL_COUNT_HORIZONTAL; ++x){
            int fallCount=tryToMakeFallsHappenInColumn(x);
            if(fallCount>0){
                anyFalls=true;
                spawnItems(x,fallCount);
            }
        }
        return anyFalls;
    }
    bool moveInThatDirectionAndReportWhetherItIsTheSameColour(game_logic::items::Coord &location, game_logic::items::Direction const direction, game_logic::items::Item::Colour const colour){
        if(location<direction){
            location += direction;
            Item const* item=grid->peek(location);
            if(item!=NULL){
                if(item->colour == colour){
                    return true;
                }
            }
        }
        return false;
    }
}