#ifndef TSL_SETTINGS_ENTITY_H
#define TSL_SETTINGS_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace tsl {

    enum class SettingsChoice {
        None,
        Quit,
    };

    class SettingsEntity : public gf::Entity {
        public:
            SettingsEntity(gf::ResourceManager& resources, SettingsChoice& choice, GameModel& model);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();
            
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            GameModel& m_model;

            gf::WidgetContainer m_container;
            gf::SpriteWidget m_quitWidget;

            gf::Texture& m_paper;
            gf::Font& m_font;
                           
    };

}

#endif // TSL_SETTINGS_ENTITY_H