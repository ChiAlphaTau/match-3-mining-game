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
                items[i][j] = new Item{i,j,items::colourFromInt((i+(j%2))%util::constants::NUMBER_OF_COLOURS)};
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
    bool coordIsValid(Coord coord){//TODO:Make coord a proper class instead?
        return
            coord.first  >=0    && coord.first  < util::constants::CELL_COUNT_HORIZONTAL &&
            coord.second >=0    && coord.second < util::constants::CELL_COUNT_VERTICAL;
    }
    Item* Grid::peek(Coord coord){
        if(coordIsValid(coord)){
            return items[coord.first][coord.second];
        }
        return NULL;
    }
    Item* Grid::claim(Coord coord){
        if(coordIsValid(coord)){
            Item* returned=items[coord.first][coord.second];
            items[coord.first][coord.second] = NULL;
            return returned;
        }
        return NULL;
    }
    void Grid::destroy(Coord coord){
        if(coordIsValid(coord) && items[coord.first][coord.second]!=NULL){
            delete items[coord.first][coord.second];
            items[coord.first][coord.second]=NULL;
        }
    }
    void Grid::give(Coord coord, Item* item){
        if(coordIsValid(coord)){
            if(items[coord.first][coord.second]!=NULL){
                destroy(coord);
            }
            items[coord.first][coord.second]=item;
        }
    }
}