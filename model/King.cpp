#include "King.hpp"

King::King(ChessColor c) : Piece(c) {
	color = c;
	type = ChessPiece::KING;
}

