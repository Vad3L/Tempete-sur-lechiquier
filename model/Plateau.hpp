#ifndef PLATEAU_H
#define PLATEAU_H

#include "Piece.hpp"
#include "Case.hpp"

#include <gf/Vector.h>

#include <vector>

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
