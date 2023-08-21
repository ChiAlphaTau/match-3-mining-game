#include "coordinate.h"

#include "constants.h"

using util::constants::CELL_COUNT_HORIZONTAL;
using util::constants::CELL_COUNT_VERTICAL;


namespace game_logic::items{
    Coord::Coord(int x_val,int y_val): x(x_val), y(y_val){
        renormalise();
    }
    void Coord::renormalise(){
        if(x<0) x=0;
        else if(x>=CELL_COUNT_HORIZONTAL)x=CELL_COUNT_HORIZONTAL-1;

        if(y<0)y=0;
        else if(y>=CELL_COUNT_VERTICAL)y=CELL_COUNT_VERTICAL-1;
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
    Coord Coord::operator +(const Direction direction)const{
        switch(direction){
            case Direction::LEFT:           return Coord{x-1,y  };
            case Direction::RIGHT:          return Coord{x+1,y  };
            case Direction::UP:             return Coord{x  ,y-1};
            case Direction::DOWN:           return Coord{x  ,y+1};

            case Direction::NONE:default:   return Coord{x  ,y  };
        }
    }
    Coord& Coord::operator +=(const Direction direction){
        *this = (*this)+direction;
        return *this;
    }
    bool Coord::operator <(const Direction direction){
        switch(direction){
            case Direction::LEFT:           return x>0;
            case Direction::RIGHT:          return x<CELL_COUNT_HORIZONTAL-1;
            case Direction::UP:             return y>0;
            case Direction::DOWN:           return y<CELL_COUNT_VERTICAL-1;

            case Direction::NONE:default:   return true;
        }
    }
    Direction Coord::operator -(Coord const& other){
        switch(x-other.x){
            case -1:
                if((y-other.y)==0)  return Direction::LEFT;
                else                return Direction::NONE;
            case  1:
                if((y-other.y)==0)  return Direction::RIGHT;
                else                return Direction::NONE;
            case 0:
                switch(y-other.y){
                    case -1:        return Direction::UP;
                    case  1:        return Direction::DOWN;
                    default:        return Direction::NONE;
                }
            default:                return Direction::NONE;
        }
    }
    Direction Direction::operator -()const{
        switch(direction){
            case Direction::LEFT:           return Direction::RIGHT;
            case Direction::RIGHT:          return Direction::LEFT;
            case Direction::UP:             return Direction::DOWN;
            case Direction::DOWN:           return Direction::UP;

            case Direction::NONE:default:   return Direction::NONE;
        }
    }
    Direction Direction::anticlockwise()const{
        switch(direction){
            case Direction::UP:             return Direction::LEFT;
            case Direction::LEFT:           return Direction::DOWN;
            case Direction::DOWN:           return Direction::RIGHT;
            case Direction::RIGHT:          return Direction::UP;

            case Direction::NONE:default:   return Direction::NONE;
        }
    }
    Direction Direction::clockwise()const{
        switch(direction){
            case Direction::UP:             return Direction::RIGHT;
            case Direction::RIGHT:          return Direction::DOWN;
            case Direction::DOWN:           return Direction::LEFT;
            case Direction::LEFT:           return Direction::UP;

            case Direction::NONE:default:   return Direction::NONE;
        }
    }
}