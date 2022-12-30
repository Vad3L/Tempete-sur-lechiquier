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
			moveAvailable = filterMoveAuthorized(coordCaseSelected, moveAvailable);
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
				state[v.y * 8 + v.x].piece.isMoved = true;
				
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
			moveAvailable = filterMoveAuthorized(coordCaseSelected, moveAvailable);
			//std::cout << moveAvailable.size() << std::endl;
		}
	}
	return true;
}


std::vector<gf::Vector2i> Plateau::filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.x < 8);

	std::vector<gf::Vector2i> v;
	
	v = filterMoveAuthorized_Tangled_TakePion(coordCaseStart, mAvailable);
	v = filterMoveAuthorized_Check(coordCaseStart, v);
	v = addMoveBigSmallCastling(coordCaseStart, v);

	return v;
}

std::vector<gf::Vector2i> Plateau::filterMoveAuthorized_Tangled_TakePion(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	std::vector<gf::Vector2i> v;

	Piece piece = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;
	
	for(auto coordCase : mAvailable) {
		std::vector<gf::Vector2i> casesPass = piece.getCasesPass(coordCaseStart, coordCase);

		bool find = true;
		for(auto coordPass : casesPass) {
			
			int add = (piece.getColor() == ChessColor::WHITE) ? -1 : 1;
			// cas pion 
			if(piece.getType() == ChessPiece::PAWN) {
				
				if(coordPass.y == coordCaseStart.y + add ) {
					// cas eat in diagonal
					if(coordPass.x-1 >= 0){
						Piece pL = state[(coordPass.y) * 8 + coordPass.x-1].piece;
						if(pL.getType() != ChessPiece::NONE && pL.getColor() != piece.getColor()) {
							v.push_back(gf::Vector2i(coordPass.x-1, coordPass.y));
						}
					}
					
					if(coordPass.x+1 < 8){
						Piece pR = state[(coordPass.y) * 8 + coordPass.x+1].piece;
						if(pR.getType() != ChessPiece::NONE && pR.getColor() != piece.getColor()) {
							v.push_back(gf::Vector2i(coordPass.x+1, coordPass.y));
						}
					}		
				}
				
				if(state[(coordPass.y) * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) { // quelque chose devant le pion
					find=false;
					break;	
				}
			}
			
			if(state[coordPass.y * 8 + coordPass.x].piece.getType() != ChessPiece::NONE) {
			
				// cas collision couleur soi-même
				bool sameCouleur =  (state[coordPass.y * 8 + coordPass.x].piece.getColor() == piece.getColor());
				if(sameCouleur){
					find=false;
					break;
				}

				// cas collision couleur adverse
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
	
	return v;
}

std::vector<gf::Vector2i> Plateau::filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	std::vector<gf::Vector2i> v;
	
	Piece piece = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;

	for(const auto &coordCase : mAvailable) {
		
		if(coordCase.y == coordCaseStart.y  && coordCase.x == coordCaseStart.x) {
			if(!isInEchec(piece.getColor())) {
				v.push_back(coordCase);
			}
			continue;
		}

		//std::size_t sizeBf = bin.size();
		movePieces(coordCaseStart, coordCase);
		//std::size_t sizeAf = bin.size();

		if(!isInEchec(piece.getColor())) {
			v.push_back(coordCase);
		}

		deMovePieces(coordCaseStart, coordCase, sizeBf!=sizeAf);
	}

	return v;
}

std::vector<gf::Vector2i> Plateau::addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);
	std::vector<gf::Vector2i> v;
	
	Piece p = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;
	
	if(p.getType() != ChessPiece::KING || coordCaseStart.x != 4) {
		return mAvailable;
	}
	
	Piece rookD = state[coordCaseStart.y * 8 + coordCaseStart.x+3].piece;
	Piece rookG = state[coordCaseStart.y * 8 + coordCaseStart.x-4].piece;
	
	if(p.isMoved) {
		std::cout << "le roi a bouge" << std::endl;
		return mAvailable;
	}

	// small castling
	if(p.getType() == ChessPiece::KING && rookD.getType() == ChessPiece::ROOK && rookD.getColor() == p.getColor()) {
		if(!rookD.isMoved) {
			
			bool piecesExist = false;

			for(int i = 1 ; i <= 2 ; i++) {
				piecesExist |= (state[coordCaseStart.y * 8 + coordCaseStart.x+i].piece.getType() != ChessPiece::NONE);
			}
			
			if(!piecesExist) {
				v.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y));
				v.push_back(gf::Vector2i(coordCaseStart.x+2, coordCaseStart.y));
				v.push_back(coordCaseStart);

				bool inEchec = false;
				for(auto coords = v.begin() ; coords != v.end() ; coords ++ ) {
					inEchec |= caseIsInEchec(*coords, p.getColor());
				}

				if(!inEchec) {
					std::cout << "petit roque possible" << std::endl;
					mAvailable.push_back(gf::Vector2i(coordCaseStart.x+2, coordCaseStart.y));
				}
			}
		}
	}

	v.clear();

	// big castling
	if(p.getType() == ChessPiece::KING && rookG.getType() == ChessPiece::ROOK && rookG.getColor() == p.getColor()) {
		if(!rookG.isMoved) {
		
			bool piecesExist = false;

			for(int i = 1 ; i <= 3 ; i++) {
				piecesExist |= (state[coordCaseStart.y * 8 + coordCaseStart.x-i].piece.getType() != ChessPiece::NONE);
			}
			
			if(!piecesExist) {

				v.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y));
				v.push_back(gf::Vector2i(coordCaseStart.x-2, coordCaseStart.y));			
				v.push_back(coordCaseStart);

				bool inEchec = false;
				for(auto coords = v.begin() ; coords != v.end() ; coords ++ ) {
					inEchec |= caseIsInEchec(*coords, p.getColor());
				}


				if(!inEchec) {
					std::cout << "grand roque possible" << std::endl;
					mAvailable.push_back(gf::Vector2i(coordCaseStart.x-2, coordCaseStart.y));
				}
			}
		}
	}

	return mAvailable;
}

void Plateau::movePieces(gf::Vector2i coord1, gf::Vector2i coord2) {
	assert(coord1.y >= 0);
	assert(coord1.x < 8);
	assert(coord2.y >= 0);
	assert(coord2.x < 8);

	Piece p1 = state[coord1.y * 8 + coord1.x].piece;
	Piece p2 = state[coord2.y * 8 + coord2.x].piece;

	if(p1.getType() == ChessPiece::KING && abs(coord1.x - coord2.x) == 2) {
		
		std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
		if(coord1.x - coord2.x < 0) {
			std::cout << "on fait un petit roque" << std::endl;
			std::swap(state[coord1.y * 8 + 7].piece, state[coord2.y * 8 + coord2.x-1].piece);
		}else {
			std::cout << "on fait un grand roque" << std::endl;
			std::swap(state[coord1.y * 8].piece, state[coord2.y * 8 + coord2.x+1].piece);
		}
		return;
	}

	if(p2.getType() != ChessPiece::NONE) {
		bin.push_back(p2);
		state[coord2.y * 8 + coord2.x].piece = Piece(ChessColor::WHITE, ChessPiece::NONE);
	}
	
	std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
}

void Plateau::deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin) {
	assert(coord1.y >= 0);
	assert(coord1.x < 8);
	assert(coord2.y >= 0);
	assert(coord2.x < 8);

	if(inBin) {
		Piece pBin = bin[bin.size()-1]; 
		std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
		state[coord2.y * 8 + coord2.x].piece = pBin;
		bin.pop_back();
	}else {
		std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
	}
	
}

bool Plateau::isInEchec(ChessColor color) {
	// tell if player in color is in echec
	
	ChessColor colAdv = (color == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
	gf::Vector2i coordCaseKing;
	
	for(auto & caseP : state) {
		if(caseP.piece.getType() == ChessPiece::KING && caseP.piece.getColor() == color) {
			coordCaseKing = caseP.position;
			//std::cout << "coordCaseking[y,x] " << coordCaseKing.y << " " << coordCaseKing.x << std::endl<< std::endl;
			break;
		}
	}

	for(auto & caseP : state) {
		if(caseP.piece.getType() != ChessPiece::NONE && caseP.piece.getColor() == colAdv) {
			//std::cout << "cordPiecelook : " << caseP.position.y << "," << caseP.position.x << std::endl;
			std::vector<gf::Vector2i> casesAv = caseP.piece.getMoves(caseP.position);
			casesAv = filterMoveAuthorized_Tangled_TakePion(caseP.position, casesAv);
			
			

			for(auto coordPass : casesAv) {
				//std::cout << "cordPas : " << coordPass.y << "," << coordPass.x << std::endl;
				if(coordPass.y == coordCaseKing.y  && coordPass.x == coordCaseKing.x) {
					//std::cout << "En echec sa mere par " <<  (int)state[coordPass.y *8+ coordPass.x].piece.getType()<<std::endl;
					return true;
				}
			}

		}
	}

	return false;
}

bool Plateau::caseIsInEchec(gf::Vector2i coord, ChessColor color) {
	for(auto & caseP : state) {
		if(caseP.piece.getType() != ChessPiece::NONE && caseP.piece.getColor() != color) {
			//std::cout << "cordPiecelook : " << caseP.position.y << "," << caseP.position.x << std::endl;
			std::vector<gf::Vector2i> casesAv = caseP.piece.getMoves(caseP.position);
			casesAv = filterMoveAuthorized_Tangled_TakePion(caseP.position, casesAv);
			
			for(auto coordPass : casesAv) {
				
				if(coordPass.y == coord.y  && coordPass.x == coord.x) {
					//std::cout << "cordPas : " << coordPass.y << "," << coordPass.x << std::endl;
					//std::cout << "En echec sa mere par " <<  (int)state[coordPass.y *8+ coordPass.x].piece.getType()<<std::endl;
					return true;
				}
			}

		}
	}

	return false;
}