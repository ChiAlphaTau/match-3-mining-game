#pragma once

namespace game_logic::items{
    //An enum, but with member functions.
    //C.f. {https://stackoverflow.com/a/53284026}.
    class Direction{
    public:
        enum Option{LEFT,RIGHT,UP,DOWN,NONE};
    private:
        Option direction;
    public:
        Direction(Option val):direction(val) {}
        //For switch-statement-ability - can convert it to its internal enum value.
        //Also don't need to implement == or !=, as can do so by comparing the enum values.
        operator Option()const{return direction;}
        //To stop if(direction) being legal by implictly casting Direction -> Option -> bool.
        explicit operator bool() const=delete;
        Direction operator -()const;
        Direction anticlockwise()const;
        Direction clockwise()const;
    };
    class Coord{
    public:
        int x{0};
        int y{0};
        Coord(int x_val,int y_val);
        void renormalise();//Call after manually changing x&y.
        bool operator==(Coord const& other) const;
        bool isAdjacentTo(Coord const& other) const;
        Coord operator +(const Direction direction) const;
        Coord& operator +=(const Direction direction);
        bool operator <(const Direction direction) const;//Returns whether can move in that directionw without falling off the grid.
        //Returns direction to go 1 unit in to get from other to this, or NONE if no such direction exists.
        //E.g., this returns LEFT iff this is exactly one cell to the left of other (and no cells vertically).
        Direction operator -(Coord const& other) const;
    };
}
