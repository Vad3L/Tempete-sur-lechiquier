#include "Piece.hpp"


Piece::Piece(Color c, PieceEnum t){
    color = c;
    type = t;
}


Color Piece::getColor(){
    return color;
}

PieceEnum Piece::getType(){
    return type;
}        
