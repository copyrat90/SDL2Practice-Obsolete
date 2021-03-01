#include <iostream>
#include <memory>

#include "Game.hpp"

int main()
{
    Game game("03_Pong");

    if (!game.setup())
    {
        printf("setup() failed\n");
        return -1;
    }

    game.run();

    game.clean();
    
    return 0;
}