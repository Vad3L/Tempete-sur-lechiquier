#ifndef TSL_CHESS_H
#define TSL_CHESS_H

#include <gf/Vector.h>

namespace tsl {

    enum class Phase {
        NONE,
        AVANT_COUP,
        COUP,
        APRES_COUP,
        PAS_MON_TOUR,
    }; 

    enum class SubPhase {
        NONE,
        CLIQUER_CASES // plateau ou pioche // phase de chosir case pcq on a jouer une carte qui' nous l'inmpose
    };

    enum class ChessStatus {
        NO_STARTED,
        ON_GOING,
        EQUALITY,
        WIN, 
        LOOSE,
        SURRENDER,
    };

    enum class ChessPiece {
        NONE = -1,
        MIN = 0,
        KING = MIN,
        QUEEN = 1,
        BISHOP = 2,
        KNIGHT = 3,
        ROOK = 4,
        PAWN = 5,
        CAMEL = 6 ,
        PRINCE = 7,
        PRINCESS = 8,
        MAX = PRINCESS
    };
        
    enum class ChessColor { 
        NONE = -1,
        WHITE = 0,
        BLACK = 1,
        GREY = 2  
    };

    inline ChessColor operator! (const ChessColor& c) {
        if (c == ChessColor::WHITE) {
            return ChessColor::BLACK;
        }
        return ChessColor::WHITE;
    }

    inline bool inBoard(gf::Vector2i pos) {
        //assert(pos.x >= 0);
        //assert(pos.x < 8);
        //assert(pos.y >= 0);
        //assert(pos.y < 8);
        return !(pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7);
    }

}

#endif // TSL_CHESS_H