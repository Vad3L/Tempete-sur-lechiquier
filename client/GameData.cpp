#include "GameData.hpp"

GameData::GameData()
: m_myColor(ChessColor::NONE)
, m_phase()
, m_plateau()
, m_gameStatus(ChessStatus::NO_STARTED)
, m_style(0)
, m_sounds(74.f)
{
	Deck deck = Deck();
	for(Card c: deck.m_deck) {
		m_cards.insert({c.m_num, c});
	}

	std::ifstream file(std::string(GAME_CONFIGDIR)+"Settings.txt");

	if (!file) {
		gf::Log::error("Impossible d'ouvrir le fichier.\n");
	}else{
		std::string line;
	
		if(std::getline(file, line)) {
			m_style = atoi(line.c_str());	
		}
		
		if(std::getline(file, line)) {
			m_sounds = atoi(line.c_str());	
		}
		
		file.close();
	}
}

void GameData::reset() {
	m_myColor = ChessColor::NONE;
	m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
	m_phase.setCurrentSubPhase(SubPhase::NONE);
	m_plateau = Plateau();
	m_gameStatus = ChessStatus::NO_STARTED;
	m_main.fill(Card()); 
}

