#ifndef TSL_GAMEPHASE_H
#define TSL_GAMEPHASE_H

#include "Chess.h"
#include "Card.h"

namespace tsl {

    class GamePhase {
        public:
            GamePhase();

            Phase getCurrentPhase();
            SubPhase getCurrentSubPhase();
            void setCurrentPhase(Phase phase);
            void setCurrentSubPhase(SubPhase subPhase);
            void nextPhaseCard(Card cardPlay);
            std::size_t getNbCardPlay();
            std::size_t getLimNbCard();

        private:
            Phase m_currentPhase;
            SubPhase m_currentSubPhase;
            std::size_t m_nbCardPlay;
            std::size_t m_limNbCard;

    };

}

#endif // TSL_GAMEPHASE_H