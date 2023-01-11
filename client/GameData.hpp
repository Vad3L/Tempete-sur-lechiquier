#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "../model/chess.hpp"
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