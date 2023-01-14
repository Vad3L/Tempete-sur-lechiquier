#include "GamePhase.hpp"


GamePhase::GamePhase(){
    currentPhase=Phase::ATTENTE;
}

void GamePhase::applyCard(Card carte){
    Turn tour = carte.getTurn();
    Effect effet = carte.getEffect();

    switch (currentPhase){
        case Phase::CARTEAVANTCOUP:
           if(tour != Turn::BEFORE){
                return;
           }
           if(effet==Effect::CHOOSECASE){
            currentPhase=Phase::CLIQUERUNECASE;
           }
           if(tour==Turn::REPLACE){
            currentPhase=Phase::ATTENTE;
           }
           currentPhase=Phase::COUP;
           //Jouer la carte
           //carte.m_execute(plateau,);
        break;

        case Phase::CARTEAPRESCOUP:
            if(tour != Turn::AFTER){
                return;
            }
            if(effet==Effect::CHOOSECASE){
                currentPhase=Phase::CLIQUERUNECASE;
            }
            currentPhase=Phase::ATTENTE;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

        case Phase::ATTENTE:
            if(tour!= Turn::DURING){
               return;
            }
            currentPhase=Phase::CARTEAVANTCOUP;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

    }
   
}