#ifndef PLATEAU_H
#define PLATEAU_H

#include <gf/Vector.h>
#include <vector>

#include "Piece.hpp"
#include "Case.hpp"
#include "Pawn.hpp"

class Plateau {
	public:
		std::vector<Piece> bin;
		std::vector<Case> state;
		Plateau();
};

#endif // PLATEAU_H
