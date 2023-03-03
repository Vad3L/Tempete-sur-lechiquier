#ifndef TSL_MENU_ENTITY_H
#define TSL_MENU_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace tsl {

    enum class MenuChoice {
        None,
        Play,
        Rules,
        Quit,
        Settings,
    };

    class MenuEntity : public gf::Entity {
        public:
            MenuEntity(gf::ResourceManager& resources, MenuChoice& choice, GameModel& model);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();

            void update([[maybe_unused]] gf::Time time) override;
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            void renderTitleButton(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f pos, std::string title);

            GameModel& m_model;

            gf::Font& m_font;

            gf::WidgetContainer m_container;
            gf::SpriteWidget m_playWidget;
            gf::SpriteWidget m_rulesWidget;
            gf::SpriteWidget m_quitWidget;
            gf::SpriteWidget m_settingsWidget;
    };

}

#endif // TSL_MENU_ENTITY_H