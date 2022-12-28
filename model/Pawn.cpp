#include "Pawn.hpp"

Pawn::Pawn (ChessColor c) : Piece(c) {
	color = c;
        type = ChessPiece::PAWN;
}
