#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "Card.hpp"
#include "Chess.hpp"

class GamePhase{
    public:
        GamePhase();
        void applyCard(Card carte);
        void applyMove();
    private:
        Phase currentPhase;
};

#endif // GAMEPHASE_H
