#ifndef TSL_GAME_HUB_H
#define TSL_GAME_HUB_H

#include <gf/GameManager.h>
#include <gf/SegueEffects.h>

#include "AudioManager.h"
#include "CommonScene.h"
#include "DownloadScene.h"
#include "StartScene.h"
#include "MenuScene.h"
#include "ConnectionScene.h"
#include "RulesScene.h"
#include "SettingsScene.h"
#include "WaitingScene.h"
#include "GameScene.h"
#include "ZoomCardScene.h"
#include "QuitScene.h"
#include "GameModel.h"

namespace tsl {

    struct GameHub : gf::GameManager {
        GameHub();

        GameHub(const GameHub&) = delete;
        GameHub(GameHub&&) = delete;
        ~GameHub();

        GameHub& operator=(const GameHub&) = delete;
        GameHub& operator=(GameHub&&) = delete;
        
        void loadingOtherAssets(Network& network);
        bool loadingFinished();

        GameModel m_model;
        
        std::unique_ptr<CommonScene> common;
        std::unique_ptr<DownloadScene> download;
        std::unique_ptr<StartScene> start;
        std::unique_ptr<MenuScene> menu;
        std::unique_ptr<ConnectionScene> connection;
        std::unique_ptr<RulesScene> rules;
        std::unique_ptr<SettingsScene> settings;
        std::unique_ptr<WaitingScene> waiting;
        std::unique_ptr<GameScene> game;
        std::unique_ptr<ZoomCardScene> zoom;
        std::unique_ptr<QuitScene> quit;

        gf::BlackoutSegueEffect blackoutEffect;

        std::atomic_bool m_loadingFinished;
    };
}

#endif // TSL_GAME_HUB_H