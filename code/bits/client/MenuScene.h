#ifndef TSL_MENU_SCENE_H
#define TSL_MENU_SCENE_H

#include <gf/Scene.h>

#include "MenuEntity.h"

namespace tsl {

    struct GameHub;

    class MenuScene : public gf::Scene {
        public:
            MenuScene(GameHub& game);

        private:
            void doProcessEvent(gf::Event& event) override;
            void doUpdate(gf::Time time) override;

            GameHub& m_game;

            MenuChoice m_choice;
            MenuEntity m_menu;
    };
    
}

#endif // TSL_MENU_SCENE_H