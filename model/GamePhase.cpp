#include "GamePhase.hpp"


GamePhase::GamePhase(){
	m_currentPhase = Phase::AVANT_COUP;
	m_currentSubPhase = SubPhase::NONE;
	m_nbCartePlay = 0;
}

Phase GamePhase::getCurrentPhase() {
	return m_currentPhase;
}

SubPhase GamePhase::getCurrentSubPhase() {
	return m_currentSubPhase;
}


int GamePhase::getNbCartePlay() {
	return m_nbCartePlay;
}

void GamePhase::setCurrentPhase(Phase phase) {
	m_currentPhase = phase;
}

void GamePhase::setCurrentSubPhase(SubPhase subPhase) {
	m_currentSubPhase = subPhase;
}

void GamePhase::nextPhaseCard(Card cardPlay){
	assert(m_currentPhase!=Phase::COUP);
	assert(m_currentPhase !=Phase::PAS_MON_TOUR);
	m_nbCartePlay +=1;
	
	if(cardPlay.m_action == Action::NONE) {
		if(cardPlay.m_effect == Effect::NONE) {
			m_currentPhase = (Phase)((int)m_currentPhase+1);
		}else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
			m_currentPhase = Phase::PAS_MON_TOUR;
		}
	}
}
