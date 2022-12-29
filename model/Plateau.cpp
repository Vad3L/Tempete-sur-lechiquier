#include "Plateau.hpp"
#include <iostream>

Plateau::Plateau() : coordCaseSelected(-1, -1), moveAvailable() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state.push_back(Case(gf::Vector2i(j, i)));
			if (i == 1 || i == 6) {
				ChessColor c = (i == 1) ? ChessColor::BLACK : ChessColor::WHITE;
				state[i * 8 + j].piece = Piece(c, ChessPiece::PAWN);
			} else if (i == 0 || i == 7) {
				ChessColor c = (i == 0) ? ChessColor::BLACK : ChessColor::WHITE;
				switch (j) {
					case 0:
					case 7:
						state[i * 8 + j].piece = Piece(c, ChessPiece::ROOK);
						break;
					case 1:
					case 6:
						state[i * 8 + j].piece = Piece(c, ChessPiece::KNIGHT);
						break;
					case 2:
					case 5:
						state[i * 8 + j].piece = Piece(c, ChessPiece::BISHOP);
						break;
					case 3:
						state[i * 8 + j].piece = Piece(c, ChessPiece::QUEEN);
						break;
					case 4:
						state[i * 8 + j].piece = Piece(c, ChessPiece::KING);
						break;
				}
			}
		}
	}
	prettyPrint();
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

	if(coordCaseSelected.y ==-1 && coordCaseSelected.x == -1) {
		if(state[v.y *8 + v.x].piece.getColor() == color && state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) {
			coordCaseSelected = v;
			moveAvailable = state[v.y * 8 + v.x].piece.getMoves(coordCaseSelected);
			eraseCaseNonAutorized(v);
			std::cout << moveAvailable.size() << std::endl;
			return true;
		}
	}else {
		if(v.x == coordCaseSelected.x && v.y == coordCaseSelected.y) { // deselectionner case
			coordCaseSelected = gf::Vector2i(-1,-1);
			moveAvailable.clear();
			return true;
		}
		
		for(auto &coord : moveAvailable) {
			if(coord.y == v.y && coord.x == v.x) {
				// move piece
				Piece tmp = state[coordCaseSelected.y * 8 + coordCaseSelected.x].piece;
				state[coordCaseSelected.y * 8 + coordCaseSelected.x].piece = state[v.y * 8 + v.x].piece;
				state[v.y * 8 + v.x].piece = tmp;

				coordCaseSelected = gf::Vector2i(-1,-1);
				moveAvailable.clear();
				prettyPrint();
				return false;
			}
		}
		
		if(state[v.y * 8 + v.x].piece.getColor() == color && state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) { // selectionne case si la piece nous appartient
			coordCaseSelected = v;
			moveAvailable.clear();
			moveAvailable = state[v.y * 8 + v.x].piece.getMoves(coordCaseSelected);
			eraseCaseNonAutorized(v);
			std::cout << moveAvailable.size() << std::endl;
		}
	}
	return true;
}


void Plateau::eraseCaseNonAutorized(gf::Vector2i coordStart) {
	std::vector<gf::Vector2i> copy;
	for(auto coordCase : moveAvailable) {
		std::vector<gf::Vector2i> casesPass = state[coordStart.y * 8 + coordStart.x].piece.getCasesPass(coordStart, coordCase);

		bool find = true;
		for(auto j : casesPass) {
			if(state[j.y * 8 + j.x].piece.getType() != ChessPiece::NONE) {
				if(state[j.y * 8 + j.x].piece.getColor() == state[coordStart.y * 8 + coordStart.x].piece.getColor()){
					find=false;
					break;
				}
			}
		}

		if(find){
			copy.push_back(coordCase);
		}
	}
	moveAvailable = copy;
}