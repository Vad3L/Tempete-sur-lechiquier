#include "GameHub.hpp"

#define GAME_DATADIR "../data/"


GameHub::GameHub()
: GameManager("Storm on the board", { GAME_DATADIR })
, start(*this)
, menu(*this)
, play(*this)
, game(*this)
{
pushScene(start);
}


