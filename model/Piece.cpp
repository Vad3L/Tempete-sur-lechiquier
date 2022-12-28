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
        case ChessPiece::ROOK:
			newpos = getMoveRook(pos);
			break;
        case ChessPiece::KING:
			newpos = getMoveKing(pos);
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
	return newpos;
}

std::vector<gf::Vector2i> Piece::getMoveRook (gf::Vector2i pos) {
	std::vector<gf::Vector2i> newpos;
    int ligne = pos.y;
    int col = pos.x;

    for (int i = 0 ; i < 8 ; ++i) {
        if(col != i) {
            newpos.push_back(gf::Vector2i(i, ligne));
        }
        if(ligne != i) {
            newpos.push_back(gf::Vector2i(col, i));
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
            if(!(i == 0 && j == 0)) {
                newpos.push_back(gf::Vector2i(col-i, ligne-j));
            }
        }
        
    }

	return newpos;
}

