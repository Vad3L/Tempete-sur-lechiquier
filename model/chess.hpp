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
	PRINCE = 7
};
    
enum class ChessColor { 
	NONE = -1,
	WHITE = 0,
	BLACK = 1,
	GRAY = 2   
};

#endif // ECHEC_H
