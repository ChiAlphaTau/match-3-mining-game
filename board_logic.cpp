#include "board_logic.h"

#include "game.h"
#include "coordinate.h"
#include "constants.h"
#include "falling.h"
#include "item_vanilla.h"
#include "item_specials.h"

#include <vector>
#include <iostream>//TODO:remove this and get rid of item matching console messages.

using game_logic::game::grid;
using game_logic::items::Coord;
using game_logic::items::Direction;
typedef game_logic::items::Item::Colour Colour;
using game_logic::items::ItemState;
using util::constants::CELL_COUNT_HORIZONTAL;
using util::constants::CELL_COUNT_VERTICAL;

namespace game_logic::game::board_update{
    class Consecutive{
    public:
        Coord start;
        bool downInsteadOfRight;
        Colour colour;
        int length;
        int spotsFree;
        std::vector<bool> taken;
        Direction direction()const{
            return downInsteadOfRight ? Direction::DOWN : Direction::RIGHT;
        }
        void takeAt(int index){
            taken[index]=true;
            --spotsFree;
        }
        Consecutive(Coord const start_val, bool const downInsteadOfRight_val, Colour const colour_val, int const length_val):
            start{start_val}, downInsteadOfRight{downInsteadOfRight_val}, colour{colour_val}, length{length_val},
            spotsFree{length_val}, taken{std::vector<bool>(length_val,false)}{ }
    };
    void tryToIntersect(Consecutive hori, Consecutive vert){
        if(hori.start.x <= vert.start.x && vert.start.x < (hori.start.x+hori.length) &&
           vert.start.y <= hori.start.y && hori.start.y < (vert.start.y+vert.length)){
            //The two intersect.
            hori.takeAt(vert.start.x-hori.start.x);
            vert.takeAt(hori.start.y-vert.start.y);
            //Spawn a star at Coord{vert.start.x,hori.start.y} of colour vert.colour (which equals hori.colour).
            grid->give(Coord{vert.start.x,hori.start.y},new game_logic::items::Star(vert.colour));
            std::cout << "Spawn a '"<<vert.colour<<"' star at ("<<vert.start.x<<","<<hori.start.y<<").\n";
        }
    }
    void generateStars(std::vector<Consecutive> const& horis, std::vector<Consecutive> const& verts){
        for(auto hori:horis){
            for(auto vert:verts){
                tryToIntersect(hori,vert);
            }
        }
    }
    void generateRainbows(Consecutive & run){
        for(int runLength=5;runLength<=run.length && run.spotsFree>0;++runLength){
            int index{0};
            if(run.length%2!=0){//If odd.
                int const middle{(run.length-1)/2};
                if(!run.taken[middle])  index=middle;
                else{
                    for(int shift=0;true;++shift){
                        if     (!run.taken[middle-shift])   {index=middle-shift; break;}
                        else if(!run.taken[middle+shift])   {index=middle+shift; break;}
                    }
                }
            }
            else{
                int const middleRoundDown=run.length/2 - 1;
                for(int shift=0;true;++shift){
                    if     (!run.taken[middleRoundDown  -shift])    {index=middleRoundDown  -shift; break;}
                    else if(!run.taken[middleRoundDown+1+shift])    {index=middleRoundDown+1+shift; break;}
                }
            }
            Coord spawnPoint{run.start};
            Direction direction=run.direction();
            for(int i=0;i<index;++i)    spawnPoint+=direction;

            //Spawn rainbow at spawnPoint.
            grid->give(spawnPoint,new game_logic::items::Rainbow());
            std::cout << "Spawn a rainbow at ("<<spawnPoint.x<<","<<spawnPoint.y<<").\n";
            run.takeAt(index);
        }
    }
    void generateBombs(Consecutive & run){
        if(run.length==4 && run.spotsFree>0){
            for(int index=run.length-1; index>=0; --index){
                if(!run.taken[index]){
                    Coord spawnPoint{run.start};
                    Direction direction=run.direction();
                    for(int step=0;step<index;++step)   spawnPoint+=direction;
                    //Spawn bomb at spawnPoint of colour run.colour.
                    grid->give(spawnPoint,new game_logic::items::Bomb(run.colour));
                    std::cout << "Spawn a '"<<run.colour<<"' bomb at ("<<spawnPoint.x<<","<<spawnPoint.y<<").\n";
                    run.takeAt(index);
                    break;
                }
            }
        }
    }

    bool tryToFindMatchesInDirection(bool const downInsteadOfRight, bool (&shouldGo)[CELL_COUNT_HORIZONTAL][CELL_COUNT_VERTICAL], std::vector<Consecutive>& runs){
        //I shall be commenting as if forward is vertical.
        Direction const forward  = downInsteadOfRight ? Direction::DOWN  : Direction::RIGHT;
        Direction const sideways = downInsteadOfRight ? Direction::RIGHT : Direction::DOWN;
        bool foundAny{false};

        Coord start{0,0};
        while(true){
            Coord checking{start};
            while(checking<forward){//While the cell that you are checking has at least one cell after it.
                while(grid->peek(checking)==NULL){//Find the next non-null grid entry in this column. If no more exist, then continue outer for.
                    if(checking<forward)   checking+=forward;
                    else                    goto nextSidewaysLoopIteration;
                }
                Colour const colour=grid->peek(checking)->colour;//Get the colour of the item.
                Coord const runStart{checking};//Record where the potential run started, for generating the consecutive.
                if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){//If the next in the column is the same colour.
                    if(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){//Then also check the one after. If it is also the same colour, then we have a match.
                        //Record that found some.
                        foundAny=true;
                        int runLength{3};
                        //Mark the 3 found items on the grid.
                            shouldGo[checking.x][checking.y]=true;
                        Coord recent=checking+-forward;
                            shouldGo[recent.x][recent.y]=true;
                        recent+=-forward;
                            shouldGo[recent.x][recent.y]=true;
                        //If the match extends beyond, then mark those too.
                        while(moveInThatDirectionAndReportWhetherItIsTheSameColour(checking,forward,colour)){
                            shouldGo[checking.x][checking.y]=true;
                            ++runLength;
                        }
                        std::cout << "A run of length '"<<runLength<<"'.\n";
                        runs.push_back(Consecutive(runStart, downInsteadOfRight, colour, runLength));
                    }
                }
            }
            
        nextSidewaysLoopIteration:
        if(start<sideways)  start+=sideways;
        else                break;
        }

        return foundAny;
        
    }
    bool tryToDetonate(){
        bool anyDetonations{false};
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                Item* item = grid->peek(coord);
                if(item!=nullptr && item->state()==ItemState::WISHES_TO_DETONATE){
                    game_logic::game::detonatePrimedItemInGrid(item,coord);
                    anyDetonations=true;
                }
            }
        }
        return anyDetonations;
    }
    //Afterwards, shouldGo marks which were detonated.
    void removeMatches( bool (&shouldGo)[CELL_COUNT_HORIZONTAL][CELL_COUNT_VERTICAL]){
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                if(shouldGo[coord.x][coord.y]){
                    Item* item = grid->peek(coord);
                    if(item != nullptr){//Everything involved in a match is breakable as matchable, so no need to check.
                        if(game_logic::game::breakBreakableItemInGrid(item,item->colour,coord)){//If the item wishes to be destroyed, then do so.
                            shouldGo[coord.x][coord.y]=false;//And mark it as not to be detonated (if it didn't want to be destroyed, then it wants to be detonated).
                        }
                    }
                }
            }
        }
        for(Coord coord{0,0}; coord.x<CELL_COUNT_HORIZONTAL; ++coord.x){
            for(coord.y=0; coord.y<CELL_COUNT_VERTICAL; ++coord.y){
                if(shouldGo[coord.x][coord.y]){
                    Item* item = grid->peek(coord);//Not null as was broken in previous loop.
                    game_logic::game::detonatePrimedItemInGrid(item,coord);
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
            int const destinationY{head.y+fall};
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
            pushEffect(new game_logic::effects::Fall(game_logic::items::Coord(x,destinationY),fall,claimed));
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

        std::vector<Consecutive> horis{};
        std::vector<Consecutive> verts{};
        
        bool foundDetonations = tryToDetonate();

        bool foundMatches{false};
        foundMatches|=  tryToFindMatchesInDirection(true,shouldGo,verts);
        foundMatches|=  tryToFindMatchesInDirection(false,shouldGo,horis);

        if(foundMatches){
            removeMatches(shouldGo);
            generateStars(horis,verts);
            for(auto run:verts) generateRainbows(run);
            for(auto run:horis) generateRainbows(run);
            for(auto run:verts) generateBombs(run);
            for(auto run:horis) generateBombs(run);
        }

        return foundMatches || foundDetonations;
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
                if((item->state()==ItemState::CAN_BE_MATCHED_IN_GRID)&&item->colour == colour){
                    return true;
                }
            }
        }
        return false;
    }
}
namespace game_logic::game{
    bool breakBreakableItemInGrid(game_logic::items::Item* item, game_logic::items::Item::Colour cause, game_logic::items::Coord const coord){
        if(item->breakSelf(cause)){//If the item wishes to be destroyed, then do so.
            grid->destroy(coord);
            return true;
        }
        return false;
    }
    bool breakBreakableItemNotInGrid(game_logic::items::Item* item, game_logic::items::Item::Colour cause){
        if(item->breakSelf(cause)){//If the item wishes to be destroyed, then do so.
            delete item;
            return true;
        }
        return false;
    }
    void detonatePrimedItemInGrid(game_logic::items::Item* item, game_logic::items::Coord const coord){
        game_logic::effects::Effect* deathEffect=item->detonateSelf(coord);
        if(deathEffect!=nullptr)    pushEffect(deathEffect);
        grid->destroy(coord);//If the deathEffect claimed the item, then nothing happens. Otherwise destroys the item - the deathEffect no longer needs the item.
    }
}