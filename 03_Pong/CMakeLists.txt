cmake_minimum_required(VERSION 2.9)
project(SDL2Template)
add_definitions("-Wall -O2 -std=c++17 -pthread")

include_directories(/tmp/SDL2/include/SDL2 ./include)
link_directories(/tmp/SDL2/lib)


add_executable(a.out
    src/main.cpp
    src/Game.cpp
    src/sosim/SDL_Factory.cpp
    src/scene/ui/Button.cpp
    src/scene/ui/WinPopup.cpp
    src/scene/SelectPlayer.cpp
    src/scene/PlayScene.cpp
    src/asset/AssetPackage.cpp
)
target_link_libraries(a.out SDL2 SDL2_image SDL2_ttf SDL2_mixer)

file(COPY asset DESTINATION .)
