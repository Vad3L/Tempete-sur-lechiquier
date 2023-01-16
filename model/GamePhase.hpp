#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "Card.hpp"
#include "Chess.hpp"

class GamePhase{
    public:
        GamePhase();
        void nextPhaseCoupNormal();
        Phase getCurrentPhase();
        void setCurrentPhase(Phase phase);

    private:
        Phase m_currentPhase;
        int m_nbCarteJoue;
};

#endif // GAMEPHASE_H
