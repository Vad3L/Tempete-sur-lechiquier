#include "Case.hpp"

Case::Case (gf::Vector2i pos) {
	trap = false;
	hole = false;
	position = pos;
	piece = nullptr;
}
