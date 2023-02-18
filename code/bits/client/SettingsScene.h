#ifndef TSL_SETTINGS_SCENE_H
#define TSL_SETTINGS_SCENE_H

#include <gf/Scene.h>

#include "SettingsEntity.h"

namespace tsl {

    struct GameHub;	

    class SettingsScene : public gf::Scene {
        public:
            SettingsScene(GameHub& game);
            
        private:
            void doProcessEvent(gf::Event& event) override;
            void doUpdate(gf::Time time) override;
            void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

            GameHub& m_game;

            SettingsEntity m_settings;

            int m_theme;
            int m_music;
            int m_sound;

    };
    
}

#endif // TSL_SETTINGS_SCENE_H