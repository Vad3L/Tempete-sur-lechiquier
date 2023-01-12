#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <gf/Log.h>
#include <gf/Direction.h>

#include <cassert>
#include "../model/Chess.hpp"
#include "../model/Plateau.hpp"

#define GAME_DATADIR "../data/"
#define GAME_CONFIGDIR "../config/"


struct GameData {
    GameData();
    void reset();

    ChessColor m_myColor;
    bool m_myTurn;
    Plateau m_plateau;
    ChessStatus m_gameStatus;
    int m_style;
};





#endif // HG_LEVEL_DATA_H