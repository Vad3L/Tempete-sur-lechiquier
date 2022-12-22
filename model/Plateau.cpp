#include "Plateau.hpp"

Plateau::Plateau() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
			if (i == 1 || i == 6) {
				Color c = (i == 1) ? BLACK : WHITE;
				state[i * 8 + j].piece = Pawn(c);
			} else if (i == 0 || i == 7) {
				Color c = (i == 0) ? BLACK : WHITE;
				switch (j) {
					case 0:
					case 7:
						state[i * 8 + j].piece = Rook(c);
						break;
					case 1:
					case 6:
						state[i * 8 + j].piece = Knight(c);
						break;
					case 2:
					case 5:
						state[i * 8 + j].piece = Bishop(c);
						break;
					case 3:
						state[i * 8 + j].piece = Queen(c);
						break;
					case 4:
						state[i * 8 + j].piece = King(c);
						break;
				}
			}
		}
	}

}

