#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <gf/Log.h>
#include <gf/Direction.h>

#include <cassert>
#include "../model/Chess.hpp"
#include "../model/Plateau.hpp"
#include "../model/GamePhase.hpp"

#define GAME_DATADIR "../data/"
#define GAME_CONFIGDIR "../config/"


struct GameData {
    GameData();
    void reset();

    ChessColor m_myColor;
    Phase m_phase;
    Plateau m_plateau;
    ChessStatus m_gameStatus;
    int m_style;
    std::array<Card, 5> m_main;
};





#endif // HG_LEVEL_DATA_H