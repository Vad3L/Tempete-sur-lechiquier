#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "Card.hpp"

enum class Phase{
    AVANTCOUP,
    COUP,
    APRESCOUP,
    PASMONTOUR,
    CLIQUERUNECASE
};    //ATTENTEANNULATION,

class GamePhase{
    public:
        GamePhase();
        void traiterCard(Card carte);
    private:
        Phase currentPhase;
        

     


};

#endif // GAMEPHASE_H
