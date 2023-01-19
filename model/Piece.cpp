#include "Piece.hpp"

#include <gf/Log.h>

Piece::Piece(ChessColor c, ChessPiece t){
	color = c;
	type = t;
	isMoved = false;
}


ChessColor Piece::getColor(){
	return color;
}

ChessPiece Piece::getType(){
	return type;
}

std::vector<gf::Vector2i> Piece::getMoves (gf::Vector2i pos) {
	assert(pos.x >= 0);
	assert(pos.y >= 0);
	assert(pos.x < 8);
	assert(pos.y < 8);
	std::vector<gf::Vector2i> newpos;

	switch (type) {
		case ChessPiece::PAWN:
			newpos = getMovePawn(pos);
			break;
		case ChessPiece::KNIGHT:
			newpos = getMoveKnight(pos);
			break;
		case ChessPiece::BISHOP:
			newpos = getMoveBishop(pos);
			break;
		case ChessPiece::QUEEN:
			newpos = getMoveQueen(pos);
			break;
 		case ChessPiece::ROOK:
			newpos = getMoveRook(pos);
			break;
 	  	case ChessPiece::KING:
			newpos = getMoveKing(pos);
			break;
		case ChessPiece::CAMEL:
			newpos = getMoveCamel(pos);
			break;
		case ChessPiece::PRINCE:
			newpos = getMovePrince(pos);
			break;
		case ChessPiece::PRINCESS:
			newpos = getMovePrincess(pos);
			break;
		default:
			break;
	}

	for(const auto &coord : newpos) {
		assert(coord.x >= 0);
		assert(coord.y >= 0);
		assert(coord.x < 8);
		assert(coord.y < 8);
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMovePawn (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	int mul = (color == ChessColor::WHITE) ? -1 : 1;
	if (pos.y + (1 * mul) >=0 && pos.y + (1 * mul) < 8) {
		newpos.push_back(gf::Vector2i(pos.x, (1 * mul) + pos.y));
	}
	if (color == ChessColor::WHITE && pos.y == 6) {
		newpos.push_back(gf::Vector2i(pos.x, (2 * mul) + pos.y));
	} else if (color == ChessColor::BLACK && pos.y == 1) {
		newpos.push_back(gf::Vector2i(pos.x, (2 * mul) + pos.y));
	}
	return newpos;

}

std::vector<gf::Vector2i> Piece::getMoveKnight (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	std::vector<int> vec = { -2, -1, 1, 2 };
	for (int i : vec) {
		for (int j : vec) {
			if (abs(i) != abs(j)) {
				if (i + pos.x >= 0 && i + pos.x < 8 && j + pos.y >= 0 && j + pos.y < 8) {
					newpos.push_back(gf::Vector2i(i + pos.x, j + pos.y));
				}
			}
		}
	} 
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveBishop (gf::Vector2i pos,int nb) {
	std::vector<gf::Vector2i> newpos;
	for (int i = -nb; i <= nb; i++) {
		for (int j = -nb; j <= nb; j++) {
			if (abs(i) == abs(j) && i != 0 ) {
				if (i + pos.x >= 0 && i + pos.x < 8 && j + pos.y >= 0 && j + pos.y < 8) {
					newpos.push_back(gf::Vector2i(i + pos.x, j + pos.y));
				}
			}
		}
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveQueen (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	std::vector<gf::Vector2i> bishop = getMoveBishop(pos);
	std::vector<gf::Vector2i> rook = getMoveRook(pos);
	newpos.insert(newpos.end(), rook.begin(), rook.end());
	newpos.insert(newpos.end(), bishop.begin(), bishop.end());
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveRook (gf::Vector2i pos, int nb) {
	std::vector<gf::Vector2i> newpos;
	int ligne = pos.y;
	int col = pos.x;

	for (int i = 0 ; i < 8 ; ++i) {
		gf::Log::error("i %i , ligne %i , col %i\n",i, ligne, col );
		if(i >= col-nb && i <= col+nb ){	
			gf::Log::error("1er if\n");
			if(col != i) {
				gf::Log::error("1st insert %i , %i\n", i, ligne);
				newpos.push_back(gf::Vector2i(i, ligne));
			}	
		}
		if(i >= ligne-nb && i <= ligne+nb ){	
			gf::Log::error("2nd if\n");
			if(ligne != i) {
				gf::Log::error("2nd insert %i , %i\n", col, i	);
				newpos.push_back(gf::Vector2i(col, i));
			}
		}
	}  
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveKing (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	int ligne = pos.y;
	int col = pos.x;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if(!(i == 0 && j == 0) && col-i >= 0 && ligne-j >=0 && col-i < 8 && ligne-j < 8) {
				newpos.push_back(gf::Vector2i(col-i, ligne-j));
			}
		}
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveCamel (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	std::vector<int> vec = { -3, -2, 2, 3 };
	for (int i : vec) {
		for (int j : vec) {
			if (abs(i) != abs(j)) {
				if (i + pos.x >= 0 && i + pos.x < 8 && j + pos.y >= 0 && j + pos.y < 8) {
					newpos.push_back(gf::Vector2i(i + pos.x, j + pos.y));
				}
			}
		}
	} 
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMovePrince (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	int ligne = pos.y;
	int col = pos.x;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if(!(i == 0 && j == 0) && col-i >= 0 && ligne-j >=0 && col-i < 8 && ligne-j < 8) {
				newpos.push_back(gf::Vector2i(col-i, ligne-j));
			}
		}
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMovePrincess (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	std::vector<gf::Vector2i> bishop = getMoveBishop(pos,2);
	std::vector<gf::Vector2i> rook = getMoveRook(pos,2);
	newpos.insert(newpos.end(), rook.begin(), rook.end());
	newpos.insert(newpos.end(), bishop.begin(), bishop.end());
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPass (gf::Vector2i posStart, gf::Vector2i posEnd) {
	assert(posStart.x >= 0);
	assert(posStart.y >= 0);
	assert(posStart.x < 8);
	assert(posStart.y < 8);

	std::vector<gf::Vector2i> newpos;

	switch (type) {
		case ChessPiece::PAWN:
			newpos = getCasesPassPawn(posStart, posEnd);
			break;
		case ChessPiece::KNIGHT:
			newpos = getCasesPassKnight(posStart, posEnd);
			break;
		case ChessPiece::BISHOP:
			newpos = getCasesPassBishop(posStart, posEnd);
			break;
		case ChessPiece::QUEEN:
			newpos = getCasesPassQueen(posStart, posEnd);
			break;
		case ChessPiece::ROOK:
			newpos = getCasesPassRook(posStart, posEnd);
			break;
		case ChessPiece::KING:
			newpos = getCasesPassKing(posStart, posEnd);
			break;
		case ChessPiece::CAMEL:
			newpos = getCasesPassCamel(posStart, posEnd);
			break;
		case ChessPiece::PRINCE:
			newpos = getCasesPassPrince(posStart, posEnd);
			break;
		case ChessPiece::PRINCESS:
			newpos = getCasesPassPrincess(posStart, posEnd);
			break;
		default:
			break;
	}

	for(const auto &coord : newpos) {
		assert(coord.x >= 0);
		assert(coord.y >= 0);
		assert(coord.x < 8);
		assert(coord.y < 8);
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassPawn (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	int mul = (color == ChessColor::WHITE) ? -1 : 1;
	for(std::size_t i = 1 ; i <= abs(posEnd.y-posStart.y) ; i++) {
		newpos.push_back(gf::Vector2i(posStart.x, posStart.y + mul* i));
	}
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassKnight (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	newpos.push_back(posEnd);
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassBishop (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	int mulx = (posStart.x < posEnd.x) ? 1 : -1;
	int muly = (posStart.y < posEnd.y) ? 1 : -1;
	
	for(std::size_t i = 1; i <= abs(posEnd.x-posStart.x) ; i++) { 
		newpos.push_back(gf::Vector2i(posStart.x+i*mulx,posStart.y+i*muly));
	}
	
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassQueen (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	if(posStart.y != posEnd.y && posStart.x != posEnd.x) {
		std::vector<gf::Vector2i> bishop = getCasesPassBishop(posStart, posEnd);
		newpos.insert(newpos.end(), bishop.begin(), bishop.end());
	}else {
		std::vector<gf::Vector2i> rook = getCasesPassRook(posStart, posEnd);
		newpos.insert(newpos.end(), rook.begin(), rook.end());
	}

	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassRook (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	if(posStart.y == posEnd.y) {
		if(posStart.x < posEnd.x) {
			for(int i = posStart.x+1; i <= posEnd.x ; i++) {
				newpos.push_back(gf::Vector2i(i, posStart.y));
			}
		}else {
			for(int i = posEnd.x ; i < posStart.x ; i++) {
				newpos.push_back(gf::Vector2i(i, posStart.y));
			}
		}
	}else {
		if(posStart.y < posEnd.y) {
			for(int i = posStart.y+1 ; i <= posEnd.y ; i++) {
				newpos.push_back(gf::Vector2i(posStart.x, i));
			}
		}else {
			for(int i = posEnd.y ; i < posStart.y ; i++) {
				newpos.push_back(gf::Vector2i(posStart.x, i));
			}
		}
	}
	
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassKing (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	newpos.push_back(posEnd);
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassCamel (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	newpos.push_back(posEnd);
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassPrince (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	newpos.push_back(posEnd);
	return newpos;
}

std::vector<gf::Vector2i> Piece::getCasesPassPrincess (gf::Vector2i posStart, gf::Vector2i posEnd) {
	std::vector<gf::Vector2i> newpos;
	if(posStart.y != posEnd.y && posStart.x != posEnd.x) {
		std::vector<gf::Vector2i> bishop = getCasesPassBishop(posStart, posEnd);
		newpos.insert(newpos.end(), bishop.begin(), bishop.end());
	}else {
		std::vector<gf::Vector2i> rook = getCasesPassRook(posStart, posEnd);
		newpos.insert(newpos.end(), rook.begin(), rook.end());
	}

	return newpos;
}
