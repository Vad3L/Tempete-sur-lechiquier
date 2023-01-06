#include "GameHub.hpp"

#define GAME_DATADIR "../data/"
#define GAME_CONFIGDIR "../config/"

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


