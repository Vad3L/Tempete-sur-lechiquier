#ifndef CASE_H
#define CASE_H

#include <gf/Vector.h>

class Case {
	private:
		std::unique_ptr<Piece> piece;
		bool trap;
		bool hole;
		gf::Vector2i position;
	public:
		Case (gf::Vector2i position);
};

#endif
