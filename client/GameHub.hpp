#ifndef GAME_HUB_H
#define GAME_HUB_H

#include <gf/GameManager.h>
#include <gf/Packet.h>

#include "Scene/StartScene.hpp"
#include "Scene/MenuScene.hpp"
#include "Scene/PlaySelectScene.hpp"
#include "Scene/GameScene.hpp"
#include "Scene/RulesScene.hpp"

#include "Network.hpp"

struct GameHub : gf::GameManager {
    GameHub();


    StartScene start;
    MenuScene menu;
    PlaySelectScene play;
    GameScene game;
    RulesScene rules;

    Network m_network;
    
};





#endif // GAME_HUB_H
