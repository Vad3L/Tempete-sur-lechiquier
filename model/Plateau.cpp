#include "Plateau.hpp"

Plateau::Plateau() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
		}
	}
}

