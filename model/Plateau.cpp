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
	
	Piece pSelect = state[v.y * 8 + v.x].piece;

	if(coordCaseSelected.y ==-1 && coordCaseSelected.x == -1) {
		if( pSelect.getType() != ChessPiece::NONE) { // remettre pSelect.getColor() == color &&
			coordCaseSelected = v;
			moveAvailable = pSelect.getMoves(coordCaseSelected);
			eraseCaseNoAuthorized();
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
				movePieces(coordCaseSelected, v);

				coordCaseSelected = gf::Vector2i(-1,-1);
				moveAvailable.clear();
				prettyPrint();
				return false;
			}
		}
		
		if( pSelect.getType() != ChessPiece::NONE) { // selectionne case si la piece nous appartient // remettre pSelect.piece.getColor() == color &&
			coordCaseSelected = v;
			moveAvailable.clear();
			moveAvailable = pSelect.getMoves(coordCaseSelected);
			eraseCaseNoAuthorized();
			//std::cout << moveAvailable.size() << std::endl;
		}
	}
	return true;
}


void Plateau::eraseCaseNoAuthorized() {
	std::vector<gf::Vector2i> v;
	Piece pSelected = state[coordCaseSelected.y * 8 + coordCaseSelected.x].piece;
	
	for(auto coordCase : moveAvailable) {
		std::vector<gf::Vector2i> casesPass = pSelected.getCasesPass(coordCaseSelected, coordCase);

		bool find = true;
		for(auto coordPass : casesPass) {
			
			// cas pion 
			if(pSelected.getType() == ChessPiece::PAWN) {
				Piece pL = state[(coordPass.y) * 8 + coordPass.x-1].piece;
				Piece pR = state[(coordPass.y) * 8 + coordPass.x+1].piece;
				
				if(pL.getType() != ChessPiece::NONE && pL.getColor() != pSelected.getColor()) {
					v.push_back(gf::Vector2i(coordPass.x-1, coordPass.y));
				}
				if(pR.getType() != ChessPiece::NONE && pR.getColor() != pSelected.getColor()) {
					v.push_back(gf::Vector2i(coordPass.x+1, coordPass.y));
				}	

				if(state[(coordPass.y) * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) { // quelque chose devant le pion
					find=false;
					break;	
				}
			}

			if(state[coordPass.y * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) {
			
				// cas collsion couleur soi-même
				bool sameCouleur =  (state[coordPass.y * 8 + coordPass.x].piece.getColor() == pSelected.getColor());
				if(sameCouleur){
					find=false;
					break;
				}

				// cas collsion couleur adverse
				if(!sameCouleur && !(coordCase.y == coordPass.y && coordCase.x == coordPass.x)){
					find=false;
					break;
				}
			}
		}

		if(find){
			v.push_back(coordCase);
		}
	}
	moveAvailable = v;
}

bool Plateau::isInEchec(ChessColor color) {
	ChessColor adv = (color == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;

	bool isEchec = false;

	return isEchec;
}

void Plateau::movePieces(gf::Vector2i coord1, gf::Vector2i coord2) {
	assert(coord1.y >= 0);
	assert(coord1.x < 8);
	assert(coord2.y >= 0);
	assert(coord2.x < 8);

	Piece p1 = state[coord1.y * 8 + coord1.x].piece;
	Piece p2 = state[coord2.y * 8 + coord2.x].piece;

	if(p2.getType() != ChessPiece::NONE) {
		bin.push_back(p2);
		state[coord2.y * 8 + coord2.x].piece = Piece(ChessColor::WHITE, ChessPiece::NONE);
	}
	
	std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
}