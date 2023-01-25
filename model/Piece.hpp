#ifndef PIECE_H
#define PIECE_H

#include <gf/Vector.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "Chess.hpp"

class Piece  {
	public:
		bool isMoved;

	public:
		Piece(ChessColor c, ChessPiece t);

		ChessColor getColor();
		ChessPiece getType();

		std::vector<gf::Vector2i> getMoves (gf::Vector2i pos);

		std::vector<gf::Vector2i> getMovePawn (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMoveKnight (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMoveBishop (gf::Vector2i pos,int nb = 8);
		std::vector<gf::Vector2i> getMoveQueen (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMoveRook (gf::Vector2i pos,int nb = 8);
		std::vector<gf::Vector2i> getMoveKing (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMoveCamel (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMovePrince (gf::Vector2i pos);
		std::vector<gf::Vector2i> getMovePrincess (gf::Vector2i pos);

		std::vector<gf::Vector2i> getCasesPass (gf::Vector2i posStart, gf::Vector2i posEnd);

		std::vector<gf::Vector2i> getCasesPassPawn (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassKnight (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassBishop (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassQueen (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassRook (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassKing (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassCamel (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassPrince (gf::Vector2i posStart, gf::Vector2i posEnd);
		std::vector<gf::Vector2i> getCasesPassPrincess (gf::Vector2i posStart, gf::Vector2i posEnd);
		
		bool operator== (const Piece &p) const;

	protected:
		ChessColor color;
		ChessPiece type;
};

#endif // PIECE_H
