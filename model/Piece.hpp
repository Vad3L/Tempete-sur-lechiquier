#ifndef PIECE_H
#define PIECE_H

#include <gf/Vector.h>

#include <cstdlib>
#include <vector>

#include "echec.hpp"

class Piece  {
    public:
        
        /**
        * Build  Piece
        */
        Piece(ChessColor c, ChessPiece t);

        /**
        * Guetteur on the color
        */
        ChessColor getColor();

        /**
        * Guetteur on the Type of the piece
        */
        ChessPiece getType();        

	std::vector<gf::Vector2i> getMoves (gf::Vector2i pos);

	std::vector<gf::Vector2i> getMovePawn (gf::Vector2i pos);
	std::vector<gf::Vector2i> getMoveKnight (gf::Vector2i pos);
	std::vector<gf::Vector2i> getMoveBishop (gf::Vector2i pos);
	std::vector<gf::Vector2i> getMoveQueen (gf::Vector2i pos);
    protected:
        ChessColor color;
        ChessPiece type;
};

#endif // PIECE_H
