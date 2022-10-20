#include "GameHub.hpp"

#include "../../config.h"

namespace game{    

  GameHub::GameHub()
  : GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
  , start(*this)
  , menu(*this)

  {
    pushScene(start);
  }


}
