#include "GameHub.hpp"


GameHub::GameHub()
: GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
, start(*this)
, menu(*this)
, play(*this)
, game(*this)
, rules(*this)
, quit(*this)
{
pushScene(start);
}


