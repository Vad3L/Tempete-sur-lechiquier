#include "CardFunction.hpp"

bool Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	Case c = p.state[s.y * 8 + s.x];
	if (c.piece.getType() != ChessPiece::KNIGHT) {
		return false;
	}

	ChessColor clr = c.piece.getColor();
	c.piece = Piece(clr, ChessPiece::CAMEL);
	return true;
}

bool QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	inBoard(e);

	Case c = p.state[s.y * 8 + s.x];
	Case d = p.state[e.y * 8 + e.x];

	// TODO check if corner + if three other corners are full

	if (d.piece.getType() != ChessPiece::NONE) {
		return false;
	}

	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
	return true;
}

