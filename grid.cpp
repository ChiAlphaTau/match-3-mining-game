#include "grid.h"

namespace game_logic{
    items::Item::Colour items::colourFromInt(const int val){//TODO:Move elsewhere, probably.
        if(val<util::constants::NUMBER_OF_COLOURS){
            return static_cast<items::Item::Colour>(val);
        }
        return items::Item::RED;//Maybe throw error instead?
    }
    Grid::Grid(){
        for(int i=0; i<util::constants::CELL_COUNT_HORIZONTAL; ++i){
            for(int j=0; j<util::constants::CELL_COUNT_VERTICAL; ++j){
                items[i][j] = new Item{items::colourFromInt((i+(j%2))%util::constants::NUMBER_OF_COLOURS)};
            }
        }
    }
    Grid::~Grid(){
        for(int i=0; i<util::constants::CELL_COUNT_HORIZONTAL; ++i){
            for(int j=0; j<util::constants::CELL_COUNT_VERTICAL; ++j){
                if(items[i][j]!=NULL){
                    delete items[i][j];
                    items[i][j]=NULL;
                }
            }
        }
    }
    Item* Grid::peek(Coord const& coord){
        return items[coord.x][coord.y];
    }
    Item* Grid::claim(Coord const& coord){
        Item* returned=items[coord.x][coord.y];
        items[coord.x][coord.y] = NULL;
        return returned;
    }
    void Grid::destroy(Coord const& coord){
        if(items[coord.x][coord.y]!=NULL){
            delete items[coord.x][coord.y];
            items[coord.x][coord.y]=NULL;
        }
    }
    void Grid::give(Coord const& coord, Item* item){
        if(items[coord.x][coord.y]!=NULL){
            destroy(coord);
        }
        items[coord.x][coord.y]=item;
    }
}