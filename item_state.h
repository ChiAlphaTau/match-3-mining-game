#pragma once

namespace game_logic::items{
    //An enum, but with member functions.
    //C.f. {https://stackoverflow.com/a/53284026}.
    class ItemState{
    public:
        enum Val{
            CAN_BE_MATCHED_IN_GRID,
            CANNOT_BE_MATCHED_BUT_BREAKABLE,
            WISHES_TO_DETONATE,
            DETONATED
        };
    private:
        Val internal;
    public:
        ItemState(Val val):internal(val) {}
        //Allow auto-casting to Val for comparison/switch, but disallow (auto-cast to in) if(itemState) statements.
        operator Val()const{return internal;}   explicit operator bool() const=delete;
        bool isBreakable()const{
            switch(internal){
                case CAN_BE_MATCHED_IN_GRID:case CANNOT_BE_MATCHED_BUT_BREAKABLE:
                    return true;
                default:
                    return false;
            }
        }
    };
}