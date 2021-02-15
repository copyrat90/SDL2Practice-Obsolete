cmake_minimum_required(VERSION 2.9)
project(SDL2Template)
add_definitions("-Wall -Werror -O2 -std=c++17")

include_directories(/tmp/SDL2/include/SDL2 ./include)
link_directories(/tmp/SDL2/lib)


add_executable(a.out
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/Cell.cpp
    src/sosim/SDL_Factory.cpp
)
target_link_libraries(a.out SDL2 SDL2_image SDL2_ttf SDL2_mixer)
