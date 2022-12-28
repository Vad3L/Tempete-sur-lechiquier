#include "Plateau.hpp"
#include <iostream>

Plateau::Plateau() : caseSelected(-1, -1), moveAvailable(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
			if (i == 1 || i == 6) {
				ChessColor c = (i == 1) ? ChessColor::BLACK : ChessColor::WHITE;
				state[i * 8 + j].piece = Pawn(c);
			} else if (i == 0 || i == 7) {
				ChessColor c = (i == 0) ? ChessColor::BLACK : ChessColor::WHITE;
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
	this->prettyPrint();
}

void Plateau::prettyPrint() {
	for  (Case &c : this->state) {
		std::string s = (c.piece.getType() == ChessPiece::NONE) ? "   " : "    ";
		if(c.position.x == 7) {
			if(c.piece.getColor() == ChessColor::WHITE) {
			   std::cout << "W" << (int)c.piece.getType() << s;
			   
			}else {
			   std::cout << "B" <<(int)c.piece.getType() << s;
			}
			std::cout << "\n";
		}else {
			if(c.piece.getColor() == ChessColor::WHITE) {
			   std::cout << "W" << (int)c.piece.getType() << s;
			}else {
			   std::cout << "B" <<(int)c.piece.getType() << s;
			}
		}
	}
}

bool Plateau::setMovement(ChessColor color, gf::Vector2i v) {
	
	if(v.x == -1 || v.y == -1) {
		return true;
	}

	if(caseSelected.y ==-1 && caseSelected.x == -1) {
		if(state[v.y *8 + v.x].piece.getColor() == color && state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) {
			caseSelected = v;
			//TODO calculer mouvement possible
			moveAvailable.push_back(gf::Vector2i(caseSelected.x-1, caseSelected.y-1));
			moveAvailable.push_back(gf::Vector2i(caseSelected.x+1, caseSelected.y-1));
			//moveAvailable = state[v.y *8 + v.x].piece.getMovement(caseSelected, this);
			return true;
		}
	}else {
		if(v.x == caseSelected.x && v.y == caseSelected.y) { // deselectionner case
			caseSelected = gf::Vector2i(-1,-1);
			moveAvailable.clear();
			return true;
		}
		
		for(auto &coord : moveAvailable) {
			if(coord.y == v.y && coord.x == v.x) {
				// move piece
				Piece tmp = state[caseSelected.y * 8 + caseSelected.x].piece;
				state[caseSelected.y * 8 + caseSelected.x].piece = state[v.y * 8 + v.x].piece;
				state[v.y * 8 + v.x].piece = tmp;

				caseSelected = gf::Vector2i(-1,-1);
				moveAvailable.clear();
				this->prettyPrint();
				return false;
			}
		}
		
		if(state[v.y *8 + v.x].piece.getColor() == color && state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) { // selectionne case si la piece nous appartient
			caseSelected = v;
			moveAvailable.clear();
			//TODO calculer mouvement possible
			moveAvailable.push_back(gf::Vector2i(caseSelected.x-1, caseSelected.y-1));
			moveAvailable.push_back(gf::Vector2i(caseSelected.x+1, caseSelected.y-1));
			//moveAvailable = state[v.y *8 + v.x].piece.getMovement(caseSelected, this);
		}
	}
	return true;
}
