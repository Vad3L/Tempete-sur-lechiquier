#include "Plateau.hpp"

Plateau::Plateau() {
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
		}
	}
}

