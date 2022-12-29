#include "Plateau.hpp"
#include <iostream>

Plateau::Plateau() : coordCaseSelected(-1, -1), moveAvailable() {
	for (int i = 0; i < 8; i++) {
		for (int coordPass = 0; coordPass < 8; coordPass++) {
			state.push_back(Case(gf::Vector2i(coordPass, i)));
			if (i == 1 || i == 6) {
				ChessColor c = (i == 1) ? ChessColor::BLACK : ChessColor::WHITE;
				state[i * 8 + coordPass].piece = Piece(c, ChessPiece::PAWN);
			} else if (i == 0 || i == 7) {
				ChessColor c = (i == 0) ? ChessColor::BLACK : ChessColor::WHITE;
				switch (coordPass) {
					case 0:
					case 7:
						state[i * 8 + coordPass].piece = Piece(c, ChessPiece::ROOK);
						break;
					case 1:
					case 6:
						state[i * 8 + coordPass].piece = Piece(c, ChessPiece::KNIGHT);
						break;
					case 2:
					case 5:
						state[i * 8 + coordPass].piece = Piece(c, ChessPiece::BISHOP);
						break;
					case 3:
						state[i * 8 + coordPass].piece = Piece(c, ChessPiece::QUEEN);
						break;
					case 4:
						state[i * 8 + coordPass].piece = Piece(c, ChessPiece::KING);
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
		if( state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) {//state[v.y *8 + v.x].piece.getColor() == color &&
			coordCaseSelected = v;
			moveAvailable = state[v.y * 8 + v.x].piece.getMoves(coordCaseSelected);
			eraseCaseNoAuthorized(v);
			//std::cout << moveAvailable.size() << std::endl;
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
		
		if(state[v.y *8 + v.x].piece.getType() != ChessPiece::NONE) { // selectionne case si la piece nous appartient state[v.y * 8 + v.x].piece.getColor() == color && 
			coordCaseSelected = v;
			moveAvailable.clear();
			moveAvailable = state[v.y * 8 + v.x].piece.getMoves(coordCaseSelected);
			eraseCaseNoAuthorized(v);
			//std::cout << moveAvailable.size() << std::endl;
		}
	}
	return true;
}


void Plateau::eraseCaseNoAuthorized(gf::Vector2i coordStart) {
	std::vector<gf::Vector2i> v;
	for(auto coordCase : moveAvailable) {
		std::vector<gf::Vector2i> casesPass = state[coordStart.y * 8 + coordStart.x].piece.getCasesPass(coordStart, coordCase);

		bool find = true;
		for(auto coordPass : casesPass) {
			if(state[coordPass.y * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) {
				// cas collsion couleur soi-même
				if(state[coordPass.y * 8 + coordPass.x].piece.getColor() == state[coordStart.y * 8 + coordStart.x].piece.getColor()){
					find=false;
					break;
				}

				// cas collsion couleur adverse
				if(state[coordPass.y * 8 + coordPass.x].piece.getColor() != state[coordStart.y * 8 + coordStart.x].piece.getColor() && !(coordCase.y == coordPass.y && coordCase.x == coordPass.x)){
					find=false;
					break;
				}

				// cas pion 
				if(state[coordStart.y * 8 + coordStart.x].piece.getType() == ChessPiece::PAWN ){
					if(state[(coordPass.y) * 8 + coordPass.x-1].piece.getType() != ChessPiece::NONE) {
						v.push_back(gf::Vector2i(coordPass.x-1, coordPass.y));
					}
					if(state[(coordPass.y) * 8 + coordPass.x+1].piece.getType() != ChessPiece::NONE) {
						v.push_back(gf::Vector2i(coordPass.x+1, coordPass.y));
					}	
					if(state[(coordPass.y) * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) { // quelque chose devant le pion
						find=false;
						break;	
					}
				}
			}
		}

		if(find){
			v.push_back(coordCase);
		}
	}
	moveAvailable = v;
}