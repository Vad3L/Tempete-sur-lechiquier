#include "Case.hpp"

Case::Case (gf::Vector2i pos) : piece(WHITE) {
	trap = false;
	hole = false;
	position = pos;
}
