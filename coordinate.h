#pragma once

namespace game_logic::items{
    class Coord{
    public:
        int x{0};
        int y{0};
        Coord(int x_val,int y_val);
        bool operator==(Coord const& other);
        bool isAdjacentTo(Coord const& other);
        bool moveLeft();//If can't move any further left, go to leftmost possible and return false. Otherwise go left and return true.
        bool moveRight();
        bool moveUp();
        bool moveDown();
        void renormalise();//Call after manually changing x&y.
    };
}
