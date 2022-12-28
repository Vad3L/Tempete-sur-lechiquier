#include "Knight.hpp"

Knight::Knight(ChessColor c) : Piece(c) {
	color = c;
	type = ChessPiece::KNIGHT;
}

