#ifndef PIECE_H
#define PIECE_H

#include <string>

#include "./Color.hpp"
#include "./PieceEnum.hpp"

class Piece {


    public:

        /**
        * Build  Piece
        */
        explicit Piece(Color c, PieceEnum t);

        /**
        * Guetteur on the color
        */
        Color getColor();

        /**
        * Guetteur on the Type of the piece
        */
        PieceEnum getType();        
        
    private:
        Color color;
        PieceEnum type;




};
#endif // PIECE_H