#ifndef PLATEAU_H
#define PLATEAU_H

#include <gf/Vector.h>

#include "Piece.hpp"
#include "Case.hpp"

class Plateau {
	private:
		std::vector<std::unique_ptr<Piece>> bin;
		std::vector<Case> state;
	
	public:
		Plateau();
};

#endif // PLATEAU_H
