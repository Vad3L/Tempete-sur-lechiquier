#include "GameHub.hpp"


GameHub::GameHub()
: GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
, download(std::make_unique<DownloadScene>(*this))
{
    
}

void GameHub::loadingDownloadAssets() {
    //splashScreen->loadAnimation();
}

void GameHub::loadingAssets(Network &network) {
    gf::Log::info("fin debut chargment\n");
    start = std::make_unique<StartScene>(*this);
    gf::Log::info("fin chargment start scene\n");
    menu = std::make_unique<MenuScene>(*this);
    gf::Log::info("fin chargement menu scene\n");
    rules = std::make_unique<RulesScene>(*this);
    gf::Log::info("fin chargement rules scene\n");
    play = std::make_unique<PlaySelectScene>(*this, network);
    gf::Log::info("fin chargement play scene\n");
    game = std::make_unique<GameScene>(*this, network);
    gf::Log::info("fin chargment game scene\n");
    quit = std::make_unique<QuitScene>(*this);
    gf::Log::info("fin chargment quit scene\n");
    gf::Log::info("fin chargment total\n");
    
    m_loadingFinish = true;
}

bool GameHub::loadingFinish() {
    return m_loadingFinish;
}
