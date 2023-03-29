#ifndef TSL_PLATEAU_H
#define TSL_PLATEAU_H

#include "../common/Case.h"

#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <string>

namespace tsl {

    class Board {

        public:
            Board();
            
            std::string getSymbol (ChessColor c, ChessPiece p);
            std::string getFen ();
            void prettyPrint();
            bool setMovement(gf::Vector2i v);
            
            std::vector<gf::Vector2i> filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
            std::vector<gf::Vector2i> filterMoveAuthorized_Tangled(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
            std::vector<gf::Vector2i> filterMoveAuthorized_Pawn(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
            std::vector<gf::Vector2i> filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
            std::vector<gf::Vector2i> addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable);
            void movePieces(gf::Vector2i coord1, gf::Vector2i coord2);
            void deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin);
            void promotionPiece(gf::Vector2i coordStart, ChessPiece p);

            bool isInEchec(ChessColor color, gf::Vector2i coord = gf::Vector2i(-1), gf::Vector2i caseAvoid = gf::Vector2i(-1));
            
            ChessStatus isGameOver (ChessColor c);
            void tmp(std::vector<gf::Vector2i> t);

            bool hasPromotion();
        
        public:
            std::vector<gf::Vector2i> m_moveAvailable;
            
            std::vector<Piece> m_bin;
            std::vector<Case> m_state;

            gf::Vector2i m_coordCaseSelected;
            gf::Vector2i m_coordTakePassing;
            gf::Vector2i m_caseProvokeEchec;
            bool m_promotion;
            bool m_playerInEchec;
            
            std::vector<gf::Vector2i> m_casesClicked;
   
            std::vector<gf::Vector2i> m_lastCoup;
            std::vector<std::string> m_allPositions;
            ChessColor m_turnTo;

    };

}

#endif // TSL_PLATEAU_H