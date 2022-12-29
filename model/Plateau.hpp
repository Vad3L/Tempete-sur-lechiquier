#ifndef PLATEAU_H
#define PLATEAU_H

#include "Piece.hpp"
#include "Case.hpp"

#include <gf/Vector.h>

#include <vector>
#include <cassert>

class Plateau {
	public:
		std::vector<gf::Vector2i> moveAvailable;
		gf::Vector2i coordCaseSelected;
		std::vector<Piece> bin;
		std::vector<Case> state;
		
		Plateau();
		void prettyPrint();
		bool setMovement(ChessColor color, gf::Vector2i v);
		
		void eraseCaseNoAuthorized();
		bool isInEchec(ChessColor color);
		void movePieces(gf::Vector2i coord1, gf::Vector2i coord2);
};

#endif // PLATEAU_H
