#pragma once

#include "SDL.h"//To get access to Uint8, Uint32.

namespace render::types{
    union RGB{
        struct{Uint8 r,g,b;};
        Uint8 asArray[3];
        RGB(Uint8 r_val,Uint8 g_val,Uint8 b_val): r{r_val}, g{g_val}, b{b_val} {}
        RGB(Uint32 _rgb):
            r{static_cast<Uint8>((_rgb>>16)&0xff)},
            g{static_cast<Uint8>((_rgb>> 8)&0xff)},
            b{static_cast<Uint8>((_rgb    )&0xff)} {}
    };
}