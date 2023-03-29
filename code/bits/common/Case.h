#ifndef TSL_CASE_H
#define TSL_CASE_H

#include "Piece.h"

namespace tsl {

    class Case {
        public:
            Piece m_piece;

            bool m_trap;
            bool m_hole;

            gf::Vector2i m_position;
            Case (gf::Vector2i position);
    };

}

#endif // TSL_CASE_H