#include "GameData.hpp"

#include "Singletons.hpp"

GameData::GameData()
: m_myColor(ChessColor::NONE)
, m_phase()
, m_plateau()
, m_gameStatus(ChessStatus::NO_STARTED)
, m_style(0)
{
	Deck deck = Deck();
	for(Card c: deck.m_deck) {
		m_cards.insert({c.m_num, c});
	}

	std::ifstream file(std::string(GAME_CONFIGDIR)+"Settings.txt");

	if (!file) {
		gf::Log::error("Impossible d'ouvrir le fichier settings.txt\n");
	}else{
		std::string line;
	
		if(std::getline(file, line)) {
			m_style = atoi(line.c_str());	
		}
		
		if(std::getline(file, line)) {
			BackgroundAmbiantVolume = atof(line.c_str());
			if(BackgroundAmbiantVolume == 0.f) {FxsVolume = 0.f;} 
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

