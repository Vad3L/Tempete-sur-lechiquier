#ifndef ECHEC_H
#define ECHEC_H


enum class ChessPiece {
	NONE = -1,
	PAWN = 5,
	ROOK = 4,
	BISHOP = 2,
	QUEEN = 1,
	KING = 0,
	KNIGHT = 3,
	CAMEL = 6 ,
	PRINCE
};
    
enum class ChessColor { 
	WHITE,
	BLACK,
	GRAY    
};

#endif // ECHEC_H
