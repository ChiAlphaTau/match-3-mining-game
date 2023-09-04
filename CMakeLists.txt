#To try and work out how to do this, I used various resources on the internet, including:
#   - {https://opensource.com/article/22/1/devops-cmake} and associated GitHub repository.
#   - {https://cmake.org/cmake/help/latest/guide/tutorial/index.html}
#   - {https://github.com/aminosbh/sdl2-cmake-modules}
#   - {https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/}, {https://trenki2.github.io/blog/2017/07/04/using-sdl2-image-with-cmake/} and associated GitHub repository.

#Say minimum required version of cmake (I just put my current version for this).
cmake_minimum_required(VERSION 3.18.4)

#Name the project.
project(match_3)

#Say which version of c++ is required (these 2 lines are from {https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-2-specifying-the-c-standard}; I used the same version of C++ as them).
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Add the cmake directory to the places cmake looks for modules in.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Find the 2 SDL2 libraries.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

#Add an executable that runs the main in main.cpp.
add_executable(match-3 src/main.cpp)

#Add configurations from in a subdirectory.
add_subdirectory(src)

#The executable should link to the library corresponding to the rest of the code.
target_link_libraries(match-3 PUBLIC match3Lib)
