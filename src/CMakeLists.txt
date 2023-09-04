#Create a library out of the non-main.cpp source code.
add_library(match3Lib STATIC
    abstractions/coordinate.cpp
    abstractions/grid.cpp
    assets/asset_management.cpp
    constants/constants_init.cpp
    effects/falling.cpp
    effects/swap.cpp
    items/item_bomb.cpp
    items/item_rainbow.cpp
    items/item_render.cpp
    items/item_star.cpp
    main_logic/board_logic.cpp
    main_logic/game.cpp
    program/window_admin.cpp
    rendering/render.cpp
)

#Tell that library where its .h files are, as they aren't next to them because of files being split across directories.
#Being PUBLIC means that the executable linked to this also knows to look for .h files here.
target_include_directories(match3Lib PUBLIC
    abstractions
    assets
    constants
    effects
    items
    main_logic
    program
    rendering
)

#Make it so that the that library searches for the SDL2 includes and links to the SDL2 libraries.
target_include_directories(match3Lib PUBLIC "${SDL2_INCLUDE_DIRS}" "${SDL2_IMAGE_INCLUDE_DIRS}")
target_link_libraries(match3Lib PUBLIC "${SDL2_LIBRARIES}" "${SDL2_IMAGE_LIBRARIES}")
