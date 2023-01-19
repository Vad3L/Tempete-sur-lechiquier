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

void GamePhase::nextPhaseCard(Card cardPlay){
	assert(m_currentPhase!=Phase::COUP);
	assert(m_currentPhase !=Phase::PAS_MON_TOUR);

	if(cardPlay.m_action == Action::NONE) {
		if(cardPlay.m_effect == Effect::NONE) {
			m_currentPhase = (Phase)((int)m_currentPhase+1);
		}else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
			m_currentPhase = Phase::PAS_MON_TOUR;
		}
	}

	if(cardPlay.m_action == Action::CHOOSE_CASES) {
		m_currentPhase = Phase::CLIQUER_CASES;
	}
}