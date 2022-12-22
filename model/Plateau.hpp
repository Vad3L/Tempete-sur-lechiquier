#ifndef PLATEAU_H
#define PLATEAU_H

#include <gf/Vector.h>
#include <vector>

#include "Piece.hpp"
#include "Case.hpp"

class Plateau {
	public:
		std::vector<std::unique_ptr<Piece>> bin;
		std::vector<Case> state;
		Plateau();
};

#endif // PLATEAU_H
