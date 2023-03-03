#ifndef TSL_RULES_ENTITY_H
#define TSL_RULES_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace tsl {

    enum class RulesChoice {
        None,
        Before,
        After,
        Quit,
    };

    class RulesEntity : public gf::Entity {
        public:
            RulesEntity(gf::ResourceManager& resources, RulesChoice& choice, GameModel& model);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();
            
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            GameModel& m_model;

            gf::Font& m_font;
            gf::Texture& m_paper;
                        
            gf::WidgetContainer m_container;
            gf::SpriteWidget m_leftWidget;
            gf::SpriteWidget m_rightWidget;
            gf::SpriteWidget m_quitWidget;

            std::size_t m_numPage;
    };

}

#endif // TSL_RULES_ENTITY_H