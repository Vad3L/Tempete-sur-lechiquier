#ifndef PIECE_H
#define PIECE_H

#include <memory>
#include <string>

#include "Color.hpp"
#include "PieceEnum.hpp"

class Piece {
    public:

        /**
        * Build  Piece
        */
        Piece(Color c);

        /**
        * Guetteur on the color
        */
        Color getColor();

        /**
        * Guetteur on the Type of the piece
        */
        PieceEnum getType();        
        
    protected:
        Color color;
        PieceEnum type;
};

#endif // PIECE_H
