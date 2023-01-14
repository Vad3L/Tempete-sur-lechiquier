#include "GamePhase.hpp"


GamePhase::GamePhase(){
    currentPhase=Phase::ATTENTE;
}

void GamePhase::applyCard(Card carte){
    Turn tour = carte.getTurn();
    Effect effet = carte.getEffect();

    switch (currentPhase){
        case Phase::CARTE_AVANT_COUP:
           if(tour != Turn::BEFORE){
                return;
           }
           if(effet==Effect::CHOOSECASE){
            currentPhase=Phase::CLIQUER_UNE_CASE;
           }
           if(tour==Turn::REPLACE){
            currentPhase=Phase::ATTENTE;
           }
           currentPhase=Phase::COUP;
           //Jouer la carte
           //carte.m_execute(plateau,);
        break;

        case Phase::CARTE_APRES_COUP:
            if(tour != Turn::AFTER){
                return;
            }
            if(effet==Effect::CHOOSECASE){
                currentPhase=Phase::CLIQUER_UNE_CASE;
            }
            currentPhase=Phase::ATTENTE;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

        case Phase::ATTENTE:
            if(tour!= Turn::DURING){
               return;
            }
            currentPhase=Phase::CARTE_AVANT_COUP;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

    }
   
}
