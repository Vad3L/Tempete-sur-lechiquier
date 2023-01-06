#ifndef ECHEC_H
#define ECHEC_H

enum class ChessPiece {
	NONE = -1,
	MIN = 0,
	KING = MIN,
	QUEEN = 1,
	BISHOP = 2,
	KNIGHT = 3,
	ROOK = 4,
	PAWN = 5,
	CAMEL = 6 ,
	PRINCE = 7,
	MAX = PRINCE
};
    
enum class ChessColor { 
	NONE = -1,
	WHITE = 0,
	BLACK = 1,
	GRAY = 2  
};

inline ChessColor operator! (const ChessColor& c) {
	if (c == ChessColor::WHITE) {
		return ChessColor::BLACK;
	}
	return ChessColor::WHITE;
}

#endif // ECHEC_H
