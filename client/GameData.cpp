#include "GameData.hpp"

#include <cassert>

#include <gf/Log.h>
#include <gf/Direction.h>

GameData::GameData()
: m_myColor(ChessColor::NONE)
, m_myTurn(false)
, m_plateau()
, m_style(1)
, m_gameStatus(ChessStatus::NO_STARTED)
{
    
}

void GameData::reset() {
    m_myColor = ChessColor::NONE;
    m_myTurn = false;
    m_plateau = Plateau();
    m_style = 1;
    m_gameStatus = ChessStatus::NO_STARTED; 
    
}

