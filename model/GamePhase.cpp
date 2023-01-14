#include "GamePhase.hpp"


GamePhase::GamePhase(){
    currentPhase=Phase::AVANTCOUP;
}

void GamePhase::traiterCard(Card carte){
    Turn tour = carte.getTurn();
    Effect effet = carte.getEffect();

    switch (currentPhase){
        case Phase::AVANTCOUP:
            if(tour != Turn::BEFORE){
                return;
            }
            if(effet==Effect::CHOOSECASE){
                currentPhase=Phase::CLIQUERUNECASE;
            }
            if(tour==Turn::REPLACE){
                currentPhase=Phase::PASMONTOUR;
            }
            currentPhase=Phase::COUP;
            break;

        case Phase::APRESCOUP:
            if(tour != Turn::AFTER){
                return;
            }
            if(effet==Effect::CHOOSECASE){
                currentPhase=Phase::CLIQUERUNECASE;
            }
            currentPhase=Phase::PASMONTOUR;
            break;
    } 
}