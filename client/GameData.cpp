#include "GameData.hpp"

#include <cassert>

#include <gf/Log.h>
#include <gf/Direction.h>

GameData::GameData()
: m_myColor(ChessColor::WHITE)
, m_myTurn(false)
, m_sizeSquare(56.f)
, m_plateau()
{
    
}



