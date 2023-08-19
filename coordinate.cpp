#include "coordinate.h"

#include "constants.h"

using util::constants::CELL_COUNT_HORIZONTAL;
using util::constants::CELL_COUNT_VERTICAL;


namespace game_logic::items{
    Coord::Coord(int x_val,int y_val): x(x_val), y(y_val){
        renormalise();
    }
    bool Coord::operator==(Coord const& other){
        return x==other.x && y==other.y;
    }
    bool Coord::isAdjacentTo(Coord const& other){
        return
            (x-other.x)*(x-other.x) +
            (y-other.y)*(y-other.y)
            ==1;
    }
    bool Coord::moveLeft(){
        --x;
        if(x<0){x=0;return false;}
        return true;
    }
    bool Coord::moveRight(){
        ++x;
        if(x>=CELL_COUNT_HORIZONTAL){x=CELL_COUNT_HORIZONTAL-1;return false;}
        return true;
    }
    bool Coord::moveUp(){
        --y;
        if(y<0){y=0;return false;}
        return true;
    }
    bool Coord::moveDown(){
        ++y;
        if(y>=CELL_COUNT_VERTICAL){y=CELL_COUNT_VERTICAL-1;return false;}
        return true;
    }
    void Coord::renormalise(){
        if(x<0) x=0;
        else if(x>=CELL_COUNT_HORIZONTAL)x=CELL_COUNT_HORIZONTAL-1;

        if(y<0)y=0;
        else if(y>=CELL_COUNT_VERTICAL)y=CELL_COUNT_VERTICAL-1;
    }
}