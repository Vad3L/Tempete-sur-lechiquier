#include "GamePhase.hpp"


GamePhase::GamePhase(){
    currentPhase=Phase::AVANT_COUP;
}

// next phase plutot ?????
void GamePhase::traiterCard(Card carte){
    /*Turn tour = carte.getTurn();
    Effect effet = carte.getEffect();

    switch (currentPhase){

        case Phase::AVANT_COUP:
           if(tour != Turn::BEFORE){
                return;
           }
           if(effet==Effect::CHOOSECASE){
            currentPhase=Phase::CLIQUER_UNE_CASE;
           }
           if(tour==Turn::REPLACE){
            currentPhase=Phase::PAS_MON_TOUR;
           }
           currentPhase=Phase::COUP;
           //Jouer la carte
           //carte.m_execute(plateau,);
        break;

        case Phase::APRES_COUP:
            if(tour != Turn::AFTER){
                return;
            }
            if(effet==Effect::CHOOSECASE){
                currentPhase=Phase::CLIQUER_UNE_CASE;
            }

            currentPhase=Phase::PAS_MON_TOUR;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

        case Phase::PAS_MON_TOUR:
            if(tour!= Turn::DURING){
               return;
            }
            currentPhase=Phase::AVANT_COUP;
            //Jouer la carte
            //carte.m_execute(plateau,);
        break;

    }
   */
}