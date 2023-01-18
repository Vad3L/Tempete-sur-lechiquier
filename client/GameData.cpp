#include "GameData.hpp"

GameData::GameData()
: m_myColor(ChessColor::NONE)
, m_phase()
, m_plateau()
, m_style(0)
, m_gameStatus(ChessStatus::NO_STARTED)
{
	Deck deck = Deck();
	for(Card c: deck.m_deck) {
		m_cards.insert({c.m_num, c});
	}
}

void GameData::reset() {
	m_myColor = ChessColor::NONE;
	m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
	m_plateau = Plateau();
	m_style = 0;
	m_gameStatus = ChessStatus::NO_STARTED;
	m_main.fill(Card()); 
}

