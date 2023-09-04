#include "constants_window.h"
#include "constants_grid.h"

namespace program::constants{
    const int CELL_LENGTH=64;
    const int CELL_LENGTH_HALF=CELL_LENGTH/2;
    const int WINDOW_WIDTH=CELL_LENGTH*game_logic::constants::CELL_COUNT_HORIZONTAL;
    const int WINDOW_HEIGHT=CELL_LENGTH*game_logic::constants::CELL_COUNT_VERTICAL;
}
