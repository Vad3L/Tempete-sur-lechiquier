#include "Plateau.hpp"

Plateau::Plateau() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
			if (i == 1) {
				state[i * 8 + j].piece = Pawn(BLACK);
			} else if (i == 6) {
				state[i * 8 + j].piece = Pawn(WHITE);
			}
		}
	}

}

