#include <iostream>

#include "Game.hpp"
#include "GameHub.hpp" 

int main(int argc, char* argv[]) {

    GameHub hub;
    hub.run();
    
    /*Game game(argv);
    game.run();
    */
    return EXIT_SUCCESS;
}
