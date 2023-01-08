#include "GameHub.hpp"


GameHub::GameHub()
: GameManager("Storm on chess", { GAME_DATADIR })
, start(*this)
, menu(*this)
, play(*this)
, game(*this)
, rules(*this)
{
pushScene(start);
}

void GameHub::setIp(std::string ip){
    game.setIp(ip);
}


