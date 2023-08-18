#pragma once

namespace game_logic::effects{
    class Effect{
    public:
        enum ExpiryState{
            BUSY,//Don't move onto next board state - still processing.
            IDLE,//I'm still doing stuff, so don't delete me, but my task doesn't prevent the next board state.
            DONE,//I'm finished - delete me please.
        };
        virtual int priority(){return 0;}
        virtual ExpiryState update(int dt)=0;
        virtual void draw(int dt){}
        virtual void cutShort(){}//For when the destructor is about to be called, but the effect isn't done. Have been added to effectList at this point, and grid still exits.
        virtual ~Effect(){}
    };
}