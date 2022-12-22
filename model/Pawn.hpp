#ifndef PAWB_H
#define PAWN_H

#include <string>

#include "Piece.hpp"
#include "Color.hpp"
#include "PieceEnum.hpp"

class Pawn : public Piece {
    public:
        Pawn(Color c, PieceEnum t);

};
#endif // PAWN_H
