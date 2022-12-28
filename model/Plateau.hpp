#ifndef PLATEAU_H
#define PLATEAU_H

#include <gf/Vector.h>

#include <vector>

#include "Case.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"

class Plateau {
	public:
		std::vector<gf::Vector2i> moveAvailable;
		gf::Vector2i caseSelected;
		std::vector<Piece> bin;
		std::vector<Case> state;
		
		Plateau();
		void prettyPrint();
		bool setMovement(ChessColor color, gf::Vector2i v);
};

#endif // PLATEAU_H
