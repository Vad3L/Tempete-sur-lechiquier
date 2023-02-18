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


namespace tsl {

    struct GameHub : gf::GameManager {
        GameHub();

        GameHub(const GameHub&) = delete;
        GameHub(GameHub&&) = delete;
        ~GameHub();

        GameHub& operator=(const GameHub&) = delete;
        GameHub& operator=(GameHub&&) = delete;
        
        void loadingDownloadAssets();
        void loadingOtherAssets(Network& network);
        bool loadingFinished();

        std::unique_ptr<CommonScene> common;
        std::unique_ptr<DownloadScene> download;
        std::unique_ptr<StartScene> start;
        std::unique_ptr<MenuScene> menu;
        std::unique_ptr<ConnectionScene> connection;
        std::unique_ptr<RulesScene> rules;
        std::unique_ptr<SettingsScene> settings;

        gf::BlackoutSegueEffect blackoutEffect;

        std::atomic_bool m_loadingFinished;
    };
}

#endif // TSL_GAME_HUB_H