#include "GameHub.hpp"


GameHub::GameHub()
: GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
//, download(std::make_unique<DownloadScene>(*this))
{
    getWindow().setResizable(false);  
    getWindow().setSize(gf::Vector2i(640,420));
    getWindow().setFullscreen();
}

void GameHub::loadingDownloadAssets() {
    //splashScreen->loadAnimation();
}

void GameHub::loadingAssets() {
    start = std::make_unique<StartScene>(*this);
    menu = std::make_unique<MenuScene>(*this);
    rules = std::make_unique<RulesScene>(*this);
    play = std::make_unique<PlaySelectScene>(*this);
    game = std::make_unique<GameScene>(*this);
    quit = std::make_unique<QuitScene>(*this);
    
    m_loadingFinish = true;
}

bool GameHub::loadingFinish() {
    return m_loadingFinish;
}
