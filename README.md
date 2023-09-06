# Match 3 (mining) game using C++ and SDL.
## Overview
This is primarily a project for me to learn/practice/demonstrate C++.
The plan is for this to ultimately be a non-identical clone of the [Diamond Mine game mode from Bejeweled 3 on the DS][Bejeweled-Wikipedia].

## Controls
Swap adjacent items (using the keyboard/mouse) to make matches of 3+ consecutive items of the same colour, or swap a rainbow item (it currently looks grey) with another item to set off the rainbow item.
The `wasd` keys, arrow keys and the mouse are used to move around, and enter or the mouse to (de)select an item.

## Status
Currently the core "vanilla" gameplay is there: matching, destroying, spawning, creating/detonating 3 main special items.
There currently is not any sort of points system, game end conditions, menus, nor the stuff at the bottom to dig.

In more detail, the features that are implemented include:

- Can match items by matching 3 in a row.
- They then disappear, fall, and new items spawn.
- Matching 4 or more in a row, or having matches intersecting at right-angles ('L's and 'T's) causes special items to form:
    + Matching 4 in a row generates a bomb, which destroys items in the 8 surrounding cells when it is destroyed.
    + Matches intersecting at right-angles generate a star, which destroys items in its row or column when it is destroyed.
    + Matching $n+4$ in a row, $n>0$ generates $n$ rainbows. When this is destroyed it destroys items of the colour that triggered its destruction (or all colours (excluding rainbow) if it was triggered by another rainbow).
    + If multiple of those conditions are met, then as many such special items as can fit should be generated; this bit should be implemented but is not tested much (I know it works in some cases, but I may have seen simultaneously intersecting and 5 in a row (matching a '5x3 T') not work).

## Building
I built this with CMake.
I believe the following is sufficient if you wish to simply build and run from the command line, but I have only tested it on my machine (Linux).
It may not be sufficient if you'd like stuff like IDE integration (e.g. I need extra flags for debugging and clangd support in VSCodium).

### Pre-requisits.
You should need:

- A C++ compiler that can compile C++17.
- [CMake](https://cmake.org/ "CMake").
- make (or another CMake compatible alternative).
- The libraries, including headers, for the SDL and SDL_image parts of [SDL2](https://www.libsdl.org/ "SDL"). [This link](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php "LazyFoo SDL") explains how to install the SDL bit, and [this link](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php "LazyFoo SDL_image") explains the SDL_image bit.

### Setting up the project 1st time around.
Clone this repository, or otherwise get a copy of the project source.
Let `ROOT_DIRECTORY` resolve to the root of this project (the one containing this README).
Make a folder to build it in - preferably `ROOT_DIRECTORY/build` or some subdirectory of `ROOT_DIRECTORY/build`. Let `BUILD_DIRECTORY` resolve to that build directory.
Then from within `BUILD_DIRECTORY` run
```
cmake -Wno-dev ROOT_DIRECTORY
```
The `-Wno-dev` flag is optional; it is used to suppress warnings (the warnings are to do with the module to find the SDL libraries on the system, I believe).

### Building
Each time you wish to build the project, go to `BUILD_DIRECTORY` and run
```
cmake --build .
```

### Running
To run, go to `ROOT_DIRECTORY/rundir` and run
```
`BUILD_DIRECTORY`/match-3
```

### Example
E.g. if you are currently in `ROOT_DIRECTORY` and the build directory is `ROOT_DIRECTORY/build/example`, then the above 3 commands can be accomplished with:
```
cd build/example
cmake -Wno-dev ../..
cmake --build .
cd ../../rundir
../build/example/match-3
```

## Credit
- Credit for the images is given in `rundir/images/credit.txt`.
- The cmake modules in the `cmake` directory list where they are from.
- The SDL tutorials on [LazyFoo](https://lazyfoo.net/tutorials/SDL/index.php "LazyFoo SDL contents") were useful for quickly learning the SDL-side of things; thank you to them.
- The code in `src` is by Christopher Tudball (i.e. me). See `LICENSE` for the license.

[Bejeweled-Wikipedia]: https://en.wikipedia.org/wiki/Bejeweled_3#Gameplay "Link to Bejeweled 3's Wikipedia Entry"
