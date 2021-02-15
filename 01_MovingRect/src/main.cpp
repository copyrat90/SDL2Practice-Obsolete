#include <iostream>
#include <memory>

#include "Game.hpp"

int main()
{
    Game game("sosim: SDL2 Template");

    if (!game.init())
    {
        printf("init() failed\n");
        return -1;
    }

    game.run();

    game.clean();
    
    return 0;
}