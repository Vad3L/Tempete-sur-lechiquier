#ifndef CASE_H
#define CASE_H

#include <gf/Vector.h>
#include "Piece.hpp"

class Case {
	public:
		Piece piece;
		bool trap;
		bool hole;
		gf::Vector2i position;
		Case (gf::Vector2i position);
};

#endif
