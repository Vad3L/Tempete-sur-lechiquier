#ifndef PIECE_H
#define PIECE_H

#include <gf/Vector.h>

#include <cstdlib>
#include <vector>

#include "echec.hpp"

class Piece  {
    public:
        bool isMoved;
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
	std::vector<gf::Vector2i> getMoveRook (gf::Vector2i pos);
	std::vector<gf::Vector2i> getMoveKing (gf::Vector2i pos);
    std::vector<gf::Vector2i> getMoveCamel (gf::Vector2i pos);
    std::vector<gf::Vector2i> getMovePrince (gf::Vector2i pos);


    std::vector<gf::Vector2i> getCasesPass (gf::Vector2i caseStart, gf::Vector2i caseEnd);

    std::vector<gf::Vector2i> getCasesPassPawn (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassKnight (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassBishop (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassQueen (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassRook (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassKing (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassCamel (gf::Vector2i caseStart, gf::Vector2i caseEnd);
    std::vector<gf::Vector2i> getCasesPassPrince (gf::Vector2i caseStart, gf::Vector2i caseEnd);

    protected:
        ChessColor color;
        ChessPiece type;
};

#endif // PIECE_H
