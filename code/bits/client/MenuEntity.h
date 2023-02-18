#ifndef TSL_MENU_ENTITY_H
#define TSL_MENU_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

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
            MenuEntity(gf::ResourceManager& resources, MenuChoice& choice);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();

            void update(gf::Time time) override;
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            gf::Font& m_font;

            gf::WidgetContainer m_container;
            gf::SpriteWidget m_playWidget;
            gf::SpriteWidget m_rulesWidget;
            gf::SpriteWidget m_quitWidget;
            gf::SpriteWidget m_settingsWidget;

    };

}

#endif // TSL_MENU_ENTITY_H