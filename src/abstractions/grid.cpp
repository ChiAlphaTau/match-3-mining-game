#include "grid.h"

#include "constants_grid.h"
#include "constants_colour_count.h"

namespace game_logic{
    items::Item::Colour items::colourFromInt(const int val){//TODO:Move elsewhere, probably.
        if(val<game_logic::constants::NUMBER_OF_COLOURS){
            return static_cast<items::Item::Colour>(val);
        }
        return items::Item::RAINBOW;//Maybe throw error instead?
    }
    Grid::Grid(){
        for(int i=0; i<game_logic::constants::CELL_COUNT_HORIZONTAL; ++i){
            for(int j=0; j<game_logic::constants::CELL_COUNT_VERTICAL; ++j){
                items[i][j] = nullptr;
            }
        }
    }
    Grid::~Grid(){
        for(int i=0; i<game_logic::constants::CELL_COUNT_HORIZONTAL; ++i){
            for(int j=0; j<game_logic::constants::CELL_COUNT_VERTICAL; ++j){
                if(items[i][j]!=nullptr){
                    delete items[i][j];
                    items[i][j]=nullptr;
                }
            }
        }
    }
    Item* Grid::peek(Coord const& coord){
        return items[coord.x][coord.y];
    }
    Item* Grid::claim(Coord const& coord){
        Item* returned=items[coord.x][coord.y];
        items[coord.x][coord.y] = nullptr;
        return returned;
    }
    void Grid::destroy(Coord const& coord){
        if(items[coord.x][coord.y]!=nullptr){
            delete items[coord.x][coord.y];
            items[coord.x][coord.y]=nullptr;
        }
    }
    void Grid::give(Coord const& coord, Item* item){
        if(items[coord.x][coord.y]!=nullptr){
            destroy(coord);
        }
        items[coord.x][coord.y]=item;
    }
}