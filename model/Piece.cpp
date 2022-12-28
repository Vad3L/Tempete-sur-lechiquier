#include "Piece.hpp"

Piece::Piece(ChessColor c, ChessPiece t){
    color = c;
    type = t;
}


ChessColor Piece::getColor(){
    return color;
}

ChessPiece Piece::getType(){
    return type;
}

std::vector<gf::Vector2i> Piece::getMoves (gf::Vector2i pos) {
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
	}

	return newpos;
}

std::vector<gf::Vector2i> Piece::getMovePawn (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	int mul = (color == ChessColor::WHITE) ? -1 : 1;
	if (pos.y + (1 * mul) < 8) {
		newpos.push_back(gf::Vector2i(0 + pos.x, (1 * mul) + pos.y));
	}
	if (color == ChessColor::WHITE && pos.y == 6) {
		newpos.push_back(gf::Vector2i(0 + pos.x, (2 * mul) + pos.y));
	} else if (color == ChessColor::BLACK && pos.y == 1) {
		newpos.push_back(gf::Vector2i(0 + pos.x, (2 * mul) + pos.y));
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

std::vector<gf::Vector2i> Piece::getMoveBishop (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
	for (int i = -4; i <= 4; i++) {
		for (int j = -4; j <= 4; j++) {
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
//	std::vector<gf::Vector2i> rook = getMoveRook(pos);
//	newpos.insert(bishop.begin(), bishop.end(), rook.begin(), rook.end());
	return newpos;
}

