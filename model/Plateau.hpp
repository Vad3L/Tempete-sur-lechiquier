#ifndef PLATEAU_H
#define PLATEAU_H

#include "Piece.hpp"
#include "Case.hpp"

#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include <vector>
#include <cassert>
#include <string>

class Plateau {
	public:
		std::vector<gf::Vector2i> moveAvailable;
		gf::Vector2i coordCaseSelected;
		std::vector<Piece> bin;
		std::vector<Case> state;
		bool playerInEchec;
		bool prisePassant;
		std::vector<gf::Vector2i> lastCoup;

		Plateau();
		std::string getSymbol (ChessColor c, ChessPiece p);
		void prettyPrint();
		bool setMovement(ChessColor color, gf::Vector2i v);
		
		std::vector<gf::Vector2i> filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		
		std::vector<gf::Vector2i> filterMoveAuthorized_Tangled(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> filterMoveAuthorized_Pawn(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);

		std::vector<gf::Vector2i> addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);

		void movePieces(gf::Vector2i coord1, gf::Vector2i coord2);
		void deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin);
		
		bool isInEchec(ChessColor color, gf::Vector2i coord = gf::Vector2i(-1));

		void tmp(std::vector<gf::Vector2i>);
};

#endif // PLATEAU_H
