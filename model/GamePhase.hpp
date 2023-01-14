#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "Card.hpp"

enum class Phase{
    CARTEAVANTCOUP,
    COUP,
    CARTEAPRESCOUP,
    ATTENTE,
    CLIQUERUNECASE
};    //ATTENTEANNULATION,

class GamePhase{
    public:
        GamePhase();
        void applyCard(Card carte);
        void applyMove();
    private:
        Phase currentPhase;
        

     


};

#endif // GAMEPHASE_H
