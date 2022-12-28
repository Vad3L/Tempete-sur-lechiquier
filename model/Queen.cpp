#include "Queen.hpp"

Queen::Queen(ChessColor c) : Piece(c) {
	color = c;
	type = ChessPiece::QUEEN;
}

