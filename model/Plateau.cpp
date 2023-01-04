#include "Plateau.hpp"
#include <iostream>
#include <gf/Sleep.h>
Plateau::Plateau() : coordCaseSelected(-1, -1), moveAvailable(), 
bin(), 
lastCoup(), 
playerInEchec(false),
prisePassant(false) {
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
			   
			}else if(c.piece.getColor() == ChessColor::BLACK) {
				std::cout << "B" <<(int)c.piece.getType() << s;

			}else {
				std::cout << "N" <<(int)c.piece.getType() << s;
			}
			std::cout << "\n";
		}else {
			if(c.piece.getColor() == ChessColor::WHITE) {
				std::cout << "W" << (int)c.piece.getType() << s;
			   
			}else if(c.piece.getColor() == ChessColor::BLACK) {
				std::cout << "B" <<(int)c.piece.getType() << s;

			}else {
				std::cout << "N" <<(int)c.piece.getType() << s;
			}
		}
	}
}

bool Plateau::setMovement(ChessColor color, gf::Vector2i v) {
	if(v.x == -1 || v.y == -1) {
		return false;
	}
	
	Piece pSelect = state[v.y * 8 + v.x].piece;
	ChessColor colAdv = (color == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;

	if(coordCaseSelected.y ==-1 && coordCaseSelected.x == -1) { // aucune première de selectionné
		if( pSelect.getType() != ChessPiece::NONE && pSelect.getColor() == color) { // selectionne case si la piece nous appartient 
			coordCaseSelected = v;
			moveAvailable = pSelect.getMoves(coordCaseSelected);
			moveAvailable = filterMoveAuthorized(coordCaseSelected, moveAvailable); // à commenter si on veux test la validité d'un coup par le serveur
			//std::cout << moveAvailable.size() << std::endl;
			return false;
		}
	}else {
		if(coordCaseSelected == v) { // deselectionner case
			coordCaseSelected = gf::Vector2i(-1,-1);
			moveAvailable.clear();
			return false;
		}
		
		for(auto &coord : moveAvailable) {
			if(coord.y == v.y && coord.x == v.x) {
				
				return true;
			}
		}
		
		if(pSelect.getType() != ChessPiece::NONE && pSelect.getColor() == color) { // selectionne case si la piece nous appartient 
			coordCaseSelected = v;
			moveAvailable.clear();
			moveAvailable = pSelect.getMoves(coordCaseSelected);
			moveAvailable = filterMoveAuthorized(coordCaseSelected, moveAvailable); // à commenter si on veux test la validité d'un coup par le serveur
			//std::cout << moveAvailable.size() << std::endl;
		}
	}
	return false;
}


std::vector<gf::Vector2i> Plateau::filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);

	std::vector<gf::Vector2i> v;
	
	v = filterMoveAuthorized_Tangled(coordCaseStart, mAvailable);
	v = filterMoveAuthorized_Pawn(coordCaseStart, v);
	v = filterMoveAuthorized_Check(coordCaseStart, v);
	v = addMoveBigSmallCastling(coordCaseStart, v);
	
	return v;
}

std::vector<gf::Vector2i> Plateau::filterMoveAuthorized_Tangled(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);

	std::vector<gf::Vector2i> v;

	Piece piece = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;
	
	for(auto coordCase : mAvailable) {
		std::vector<gf::Vector2i> casesPass = piece.getCasesPass(coordCaseStart, coordCase);

		bool find = true;
		for(auto coordPass : casesPass) {

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
	
	//fonction tmp debug
	tmp(v);

	return v;
}

std::vector<gf::Vector2i> Plateau::filterMoveAuthorized_Pawn(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);
	
	Piece piece = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;
	
	if(piece.getType() != ChessPiece::PAWN || coordCaseStart.y == 0 || coordCaseStart.y == 7) {
		return mAvailable;
	}
	
	int add = (piece.getColor() == ChessColor::WHITE) ? 1 : -1;
	
		
	// one case in front of pawn
	
	// cas eat in diagonal
	if(coordCaseStart.x-1 >= 0) {
		Piece pL = state[(coordCaseStart.y-add) * 8 + coordCaseStart.x-1].piece;
		if(pL.getType() != ChessPiece::NONE && pL.getColor() != piece.getColor()) {
			std::cout <<"pion peut manger diago gauche"<<std::endl;
			mAvailable.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y-add));
		}
	}
	
	if(coordCaseStart.x+1 < 8) {
		Piece pR = state[(coordCaseStart.y-add) * 8 + coordCaseStart.x+1].piece;
		if(pR.getType() != ChessPiece::NONE && pR.getColor() != piece.getColor()) {
			mAvailable.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y-add));
			std::cout <<"pion peut manger diago droite"<<std::endl;
		}
	}		

	
	if(state[(coordCaseStart.y-add) * 8 + coordCaseStart.x].piece.getType() != ChessPiece::NONE) { // quelque chose devant le pion
		auto it = std::find(mAvailable.begin(), mAvailable.end(), gf::Vector2i(coordCaseStart.x, coordCaseStart.y-add));
		if(it != mAvailable.end()) {
			std::cout << "piece devant pion"<<std::endl;
			mAvailable.erase(it);
		}
	}
	
	ChessColor tab[2] = {ChessColor::WHITE, ChessColor::BLACK};

	// prise en passant
	for(std::size_t i = 0 ; i < 2 ; i++) {
		if(piece.getColor() == tab[i] && coordCaseStart.y == 3+i && abs(lastCoup.back().y-lastCoup[lastCoup.size()-2].y) == 2 && lastCoup.back().y == 3+i) {
			if(coordCaseStart.x-1 >= 0 && lastCoup.back().x == coordCaseStart.x-1) {
				Piece pL = state[coordCaseStart.y * 8 + coordCaseStart.x-1].piece;
				if(pL.getType()==ChessPiece::PAWN && pL.getColor() != piece.getColor()) {
					std::cout << "prise en passant a gauche possible" << std::endl;
					mAvailable.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y-add));
					prisePassant = true;
				}
			}

			if(coordCaseStart.x+1 < 8 && lastCoup.back().x == coordCaseStart.x+1) {
				Piece pR = state[coordCaseStart.y * 8 + coordCaseStart.x+1].piece;
				if(pR.getType()==ChessPiece::PAWN && pR.getColor() != piece.getColor()) {
					std::cout << "prise en passant a droite possible" << std::endl;
					mAvailable.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y-add));
					prisePassant = true;
				}
			}
		}
	}
	
	//fonction tmp debug
	tmp(mAvailable);

	return mAvailable;
}

std::vector<gf::Vector2i> Plateau::filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	std::vector<gf::Vector2i> v;
	
	Piece piece = state[coordCaseStart.y * 8 + coordCaseStart.x].piece;

	for(const auto &coordCase : mAvailable) {

		std::size_t sizeBf = bin.size();
		movePieces(coordCaseStart, coordCase);
		std::size_t sizeAf = bin.size();

		if(!isInEchec(piece.getColor())) {
			v.push_back(coordCase);
		}

		deMovePieces(coordCaseStart, coordCase, sizeBf!=sizeAf);
	}

	//fonction tmp debug
	tmp(v);

	return v;
}

std::vector<gf::Vector2i> Plateau::addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
	assert(coordCaseStart.y >= 0);
	assert(coordCaseStart.y < 8);
	assert(coordCaseStart.x >= 0);
	assert(coordCaseStart.x < 8);
	
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
				for(const auto &coords : v) {
					inEchec |= isInEchec(p.getColor(), coords);
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
					inEchec |= isInEchec(p.getColor(), *coords);
				}


				if(!inEchec) {
					std::cout << "grand roque possible" << std::endl;
					mAvailable.push_back(gf::Vector2i(coordCaseStart.x-2, coordCaseStart.y));
				}
			}
		}
	}
	
	//fonction tmp debug
	tmp(mAvailable);
	
	return mAvailable;
}

void Plateau::movePieces(gf::Vector2i coord1, gf::Vector2i coord2) {
	assert(coord1.y >= 0);
	assert(coord1.x < 8);
	assert(coord2.y >= 0);
	assert(coord2.x < 8);
	assert(coord1 != coord2);
	
	Piece p1 = state[coord1.y * 8 + coord1.x].piece;
	Piece p2 = state[coord2.y * 8 + coord2.x].piece;

	//rock
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

	// eat a piece
	if(p2.getType() != ChessPiece::NONE) {
		bin.push_back(p2);
		state[coord2.y * 8 + coord2.x].piece = Piece(ChessColor::NONE, ChessPiece::NONE);
	}
	
	std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);

	// prise en passant
	if(p1.getType() == ChessPiece::PAWN && p2.getType() == ChessPiece::NONE && coord1.x != coord2.x) {
		std::cout << "on fait une prise en passant" << std::endl;
		bin.push_back(state[coord1.y*8+coord2.x].piece);
		state[coord1.y*8+coord2.x].piece = Piece(ChessColor::NONE, ChessPiece::NONE);
		return;
	}
}

void Plateau::deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin) {
	assert(coord1.y >= 0);
	assert(coord1.x < 8);
	assert(coord2.y >= 0);
	assert(coord2.x < 8);
	assert(coord1 != coord2);

	Piece p1 = state[coord1.y * 8 + coord1.x].piece;
	Piece p2 = state[coord2.y * 8 + coord2.x].piece;

	std::swap(state[coord1.y * 8 + coord1.x].piece, state[coord2.y * 8 + coord2.x].piece);
	
	if(p2.getType() == ChessPiece::PAWN && inBin && coord1.x != coord2.x && prisePassant) {
		std::cout << "on demove une prise en passant" << std::endl;
		Piece pBin = bin.back();
		state[coord1.y*8+coord2.x].piece = pBin;
		bin.pop_back();
	}else if(inBin) {
		Piece pBin = bin.back(); 
		state[coord2.y * 8 + coord2.x].piece = pBin;
		bin.pop_back();
	}
}

bool Plateau::isInEchec(ChessColor color, gf::Vector2i coord) {
	if(coord == gf::Vector2i(-1)) { // case ou on cherche l'echec du roi de couleur color
		for(auto &caseP : state) {
			if(caseP.piece.getType() == ChessPiece::KING && caseP.piece.getColor() == color) {
				coord = caseP.position;
				break;
			}
		}
	}else {// on cherche une case dont les coordonees sont en parametre
		assert(coord.y >= 0);
		assert(coord.y < 8);
		assert(coord.x >= 0);
		assert(coord.x < 8);
	}

	for(auto & caseP : state) {
		if(caseP.piece.getType() != ChessPiece::NONE && caseP.piece.getColor() != color) {
			
			std::vector<gf::Vector2i> casesAv = caseP.piece.getMoves(caseP.position);
			casesAv = filterMoveAuthorized_Tangled(caseP.position, casesAv);
			casesAv = filterMoveAuthorized_Pawn(caseP.position, casesAv);
			
			for(auto coordPass : casesAv) {
				
				if(coord == coordPass) {	
					return true;
				}
			}
		}
	}

	return false;
}

void Plateau::tmp(std::vector<gf::Vector2i> a) {
	for(const auto &it : a) {
		assert(it.x >=0);
		assert(it.x<8);
		assert(it.y >=0);
		assert(it.y<8);
	}
}