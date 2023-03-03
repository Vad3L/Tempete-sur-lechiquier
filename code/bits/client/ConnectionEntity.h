#ifndef TSL_CONNECTION_ENTITY_H
#define TSL_CONNECTION_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace tsl {

    enum class ConnectionChoice {
        None,
        Connection,
        Quit,
    };

    class ConnectionEntity : public gf::Entity {
        public:
            ConnectionEntity(gf::ResourceManager& resources, ConnectionChoice& choice, GameModel& model);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();

            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            GameModel& m_model;

            gf::WidgetContainer m_container;
            gf::SpriteWidget m_connectionWidget;
            gf::SpriteWidget m_quitWidget;

            gf::Texture& m_paper;
            gf::Font& m_font;


    };

}

#endif // TSL_CONNECTION_ENTITY_H