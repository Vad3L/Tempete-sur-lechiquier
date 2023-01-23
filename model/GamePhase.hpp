#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "../model/Card.hpp"
#include "../model/Chess.hpp"

class GamePhase {
	public:
		GamePhase();

		Phase getCurrentPhase();
		SubPhase getCurrentSubPhase();
		void setCurrentPhase(Phase phase);
		void setCurrentSubPhase(SubPhase subPhase);
		void nextPhaseCard(Card cardPlay);
		int getNbCartePlay();

	private:
		Phase m_currentPhase;
		SubPhase m_currentSubPhase;
		int m_nbCartePlay;
};

#endif // GAMEPHASE_H
