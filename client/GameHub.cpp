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
    getWindow().setResizable(false);  
    getWindow().setSize(gf::Vector2i(640,420));
    getWindow().setFullscreen();
}


