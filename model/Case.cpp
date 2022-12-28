#include "Case.hpp"

Case::Case (gf::Vector2i pos) : piece(ChessColor::WHITE) {
	trap = false;
	hole = false;
	position = pos;
}
