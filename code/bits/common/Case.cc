#include "Case.h"

namespace tsl {

    Case::Case (gf::Vector2i pos) 
    : m_piece(ChessColor::NONE, ChessPiece::NONE)
    , m_trap(false)
    , m_hole(false)
    , m_position(pos)
    {

    }

}