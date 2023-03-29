#include "GamePhase.h"

namespace tsl {

    GamePhase::GamePhase()
    : m_currentPhase(Phase::NONE)
    , m_currentSubPhase(SubPhase::NONE)
    , m_nbCardPlay(0)
    , m_limNbCard(1)
    {
        
    }

    Phase GamePhase::getCurrentPhase() {
        return m_currentPhase;
    }

    SubPhase GamePhase::getCurrentSubPhase() {
        return m_currentSubPhase;
    }

    std::size_t GamePhase::getNbCardPlay() {
        return m_nbCardPlay;
    }

    std::size_t GamePhase::getLimNbCard() {
        return m_limNbCard;
    }

    void GamePhase::setCurrentPhase(Phase phase) {
        if(phase == Phase::PAS_MON_TOUR || phase == Phase::AVANT_COUP) { // tour suivant
            m_nbCardPlay = 0;
            m_limNbCard = 1;
        }  
        m_currentPhase = phase;
    }

    void GamePhase::setCurrentSubPhase(SubPhase subPhase) {
        m_currentSubPhase = subPhase;
    }
    
    void GamePhase::nextPhaseCard(Card cardPlay){
        assert(m_currentPhase!=Phase::COUP);
        assert(m_currentPhase !=Phase::PAS_MON_TOUR);
        m_nbCardPlay +=1;
        
        if(cardPlay.m_action == Action::NONE) {
            if(cardPlay.m_effect == Effect::NONE) {
                if(m_nbCardPlay>=m_limNbCard) {
                    m_currentPhase = (Phase)((int)m_currentPhase+1);
                }		
            }else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
                m_currentPhase = Phase::PAS_MON_TOUR;
            }else if(cardPlay.m_effect == Effect::REGAME_OTHER_CARD) {
                m_limNbCard=3;
            }
        }

        if(cardPlay.m_action == Action::CHOOSE_CASES) {
            if(cardPlay.m_effect == Effect::NONE) {
                if(m_nbCardPlay>=m_limNbCard) {
                    m_currentPhase = (Phase)((int)m_currentPhase+1);
                }		
            }else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
                m_currentPhase = Phase::PAS_MON_TOUR;
            }
        }
    }

}