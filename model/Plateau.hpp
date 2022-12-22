#ifndef PLATEAU_H
#define PLATEAU_H

#include <gf/Vector.h>
#include <vector>

#include "Piece.hpp"
#include "Case.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"

class Plateau {
	public:
		std::vector<Piece> bin;
		std::vector<Case> state;
		Plateau();
};

#endif // PLATEAU_H
