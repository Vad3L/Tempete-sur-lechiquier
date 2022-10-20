#ifndef HG_GAME_HUB_HPP
#define HG_GAME_HUB_HPP

#include <gf/GameManager.h>
#include "StartScene.hpp"
#include "MenuScene.hpp"


namespace game{

    struct GameHub : gf::GameManager{
        GameHub();
        StartScene start;
        MenuScene menu;
    };

}

#endif // HG_GAME_HUB_HPP
