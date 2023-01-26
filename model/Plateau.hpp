#ifndef PLATEAU_H
#define PLATEAU_H

#include "Piece.hpp"
#include "Case.hpp"

#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/Sleep.h>
#include <gf/Log.h>

#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <functional>


class Plateau {

	public:
		Plateau();
		
		std::string getSymbol (ChessColor c, ChessPiece p);
		std::string getFen ();
		void prettyPrint();
		bool setMovement(ChessColor color, gf::Vector2i v);
		
		std::vector<gf::Vector2i> filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> filterMoveAuthorized_Tangled(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> filterMoveAuthorized_Pawn(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		std::vector<gf::Vector2i> addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
		void movePieces(gf::Vector2i coord1, gf::Vector2i coord2);
		void deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin);
		void promotionPiece(gf::Vector2i coordStart, ChessPiece p);

		bool isInEchec(ChessColor color, gf::Vector2i coord = gf::Vector2i(-1), gf::Vector2i caseAvoid = gf::Vector2i(-1));
		
		ChessStatus isGameOver (ChessColor c);
		void tmp(std::vector<gf::Vector2i> t);

	public:
		std::vector<gf::Vector2i> moveAvailable;
		
		std::vector<Piece> bin;
		std::vector<Case> state;

		gf::Vector2i coordCaseSelected;
		gf::Vector2i coordPrisePassant;
		gf::Vector2i caseProvocateEchec;
		bool m_promotion;
		bool playerInEchec;
		
		std::vector<gf::Vector2i> m_casesClicked;

		
		std::vector<gf::Vector2i> lastCoup;
		std::vector<std::string> allPositions;
		ChessColor turnTo;

};

#endif // PLATEAU_H
