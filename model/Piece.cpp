#include "Piece.hpp"


Piece::Piece(Color c){
    color = c;
    type = NONE;
}


Color Piece::getColor(){
    return color;
}

PieceEnum Piece::getType(){
    return type;
} 

