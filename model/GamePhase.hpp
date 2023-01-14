#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "Card.hpp"
#include "Chess.hpp"

class GamePhase{
    public:
        GamePhase();
        void traiterCard(Card carte);
    private:
        Phase currentPhase;
};

#endif // GAMEPHASE_H
