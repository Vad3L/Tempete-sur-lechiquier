#include "Piece.hpp"

Piece::Piece(ChessColor c){
    color = c;
    type = ChessPiece::NONE;
}


ChessColor Piece::getColor(){
    return color;
}

ChessPiece Piece::getType(){
    return type;
} 
