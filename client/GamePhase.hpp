#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "../model/Card.hpp"
#include "../model/Chess.hpp"

class GamePhase{
	public:
		GamePhase();

		Phase getCurrentPhase();
		void setCurrentPhase(Phase phase);
		void nextPhaseCard(Card cardPlay);
		int getNbCartePlay();

	private:
		Phase m_currentPhase;
		int m_nbCartePlay;
};

#endif // GAMEPHASE_H
