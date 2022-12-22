#ifndef CASE_H
#define CASE_H

#include <memory>
#include <gf/Vector.h>

#include "Piece.hpp"

class Case {
	public:
		std::unique_ptr<Piece> piece;
		bool trap;
		bool hole;
		gf::Vector2i position;
		Case (gf::Vector2i position);
};

#endif
