#ifndef TSL_BUTTONS_GAME_ENTITY_H
#define TSL_BUTTONS_GAME_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

#include "GameModel.h"

namespace tsl {

    enum class ButtonChoice {
        None,
        PlayCard,
        EndTurn,
    };

    class ButtonsGame : public gf::Entity {
        public:
            ButtonsGame(gf::ResourceManager& resources, ButtonChoice& choice, GameModel& model);
            
            void pointTo(gf::Vector2f coords);
            void triggerAction();

            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
            
        private:

            GameModel& m_model;
            
        public :
            gf::TextButtonWidget m_endTurn;
		    gf::TextButtonWidget m_playCard;
            gf::WidgetContainer m_container;
    };

}

#endif // TSL_BUTTONS_GAME_ENTITY_H