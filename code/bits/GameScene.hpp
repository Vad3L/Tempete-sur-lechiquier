#ifndef HG_GAME_HUB_HPP
#define HG_GAME_HUB_HPP

#include <gf/GameManager.h>


namespace game{

    struct GameHub : gf::GameManager{
        GameHub();
        StartScene start;
        MenuScene menu;
        Game game;
        
    };

}

#endif // HG_GAME_HUB_HPP
