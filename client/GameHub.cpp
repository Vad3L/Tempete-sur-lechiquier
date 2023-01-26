#include "GameHub.hpp"


GameHub::GameHub()
: GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
, download(std::make_unique<DownloadScene>(*this))
{	
	
}

void GameHub::loadingDownloadAssets() {
	//splashScreen->loadAnimation();
}

void GameHub::loadingAssets(Network &network, GameData &data) {
	gf::Log::info("debut chargement\n");
	start = std::make_unique<StartScene>(*this);
	gf::Log::info("fin chargement start scene\n");
	menu = std::make_unique<MenuScene>(*this);
	gf::Log::info("fin chargement menu scene\n");
	rules = std::make_unique<RulesScene>(*this);
	gf::Log::info("fin chargement rules scene\n");
	play = std::make_unique<PlaySelectScene>(*this, network);
	gf::Log::info("fin chargement play scene\n");
	game = std::make_unique<GameScene>(*this, network, data);
	gf::Log::info("fin chargement game scene\n");
	quit = std::make_unique<QuitScene>(*this);
	gf::Log::info("fin chargement quit scene\n");
	settings = std::make_unique<SettingsScene>(*this, data);
	gf::Log::info("fin chargement settings scene\n");
	zoomCard = std::make_unique<ZoomCardScene>(*this);
	gf::Log::info("fin chargement zoom card scene\n");
	gf::Log::info("fin chargement total\n");
	
	m_loadingFinish = true;
}

bool GameHub::loadingFinish() {
	return m_loadingFinish;
}
