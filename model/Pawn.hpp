#ifndef PAWN_H
#define PAWN_H

#include <string>

#include "Piece.hpp"
#include "Color.hpp"
#include "PieceEnum.hpp"

class Pawn : public Piece {
    public:
        Pawn(Color c);

};
#endif // PAWN_H
