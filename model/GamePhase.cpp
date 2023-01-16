#include "GamePhase.hpp"


GamePhase::GamePhase(){
    m_currentPhase = Phase::PAS_MON_TOUR;
    m_nbCarteJoue = 0;
}


Phase GamePhase::getCurrentPhase() {
    return m_currentPhase;
}

void GamePhase::setCurrentPhase(Phase phase) {
    if(phase == Phase::PAS_MON_TOUR || phase == Phase::AVANT_COUP) { // tour suivant
        m_nbCarteJoue = 0;
    }   
    m_currentPhase = phase;
}

void GamePhase::nextPhaseCoupNormal(){
    if(m_currentPhase == Phase::PAS_MON_TOUR) {
        return;
    }

    if(m_currentPhase == Phase::COUP && m_nbCarteJoue!=0) {
        m_currentPhase = Phase::PAS_MON_TOUR;
    }else {
        m_currentPhase = Phase::APRES_COUP;
    }
}

void GamePhase::nextPhaseBtn() {
    // btn active que si c'est mon tour
    assert(m_currentPhase != Phase::PAS_MON_TOUR);
    assert(m_currentPhase == Phase::CLIQUER_CASES || m_currentPhase == Phase::APRES_COUP);

    if(m_currentPhase == Phase::APRES_COUP) { // tour suivant on vaut pas jouer de carte
        m_currentPhase = Phase::PAS_MON_TOUR;
    } 

    if(m_currentPhase == Phase::CLIQUER_CASES) { 
        //
    } 

}