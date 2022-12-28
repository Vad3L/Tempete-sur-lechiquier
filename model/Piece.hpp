#ifndef PIECE_H
#define PIECE_H

#include <gf/Vector.h>

#include <vector>

#include "echec.hpp"

class Piece  {
    public:
        
        /**
        * Build  Piece
        */
        Piece(ChessColor c);

        /**
        * Guetteur on the color
        */
        ChessColor getColor();

        /**
        * Guetteur on the Type of the piece
        */
        ChessPiece getType();        
        
    protected:
        ChessColor color;
        ChessPiece type;
};

#endif // PIECE_H
