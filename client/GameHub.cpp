#include "GameHub.hpp"

#define GAME_DATADIR "/home/xadel/Project/Tempete-sur-lechiquier/data/"


GameHub::GameHub()
: GameManager("Storm on the board", { GAME_DATADIR })
, start(*this)
, menu(*this)
, play(*this)
, game(*this)
{
pushScene(start);
}


