#ifndef TSL_ZOOM_CARD_SCENE_H
#define TSL_ZOOM_CARD_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Event.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/RenderTarget.h>

#include "CardEntity.h"

namespace tsl {

    struct GameHub;

    class ZoomCardScene : public gf::Scene {
        public:
            ZoomCardScene(GameHub& game);

            int m_numCard;
            
        private:
            void doProcessEvent([[maybe_unused]] gf::Event& event) override;
            void doHandleActions(gf::Window& window) override;
            void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
            void onActivityChange(bool active)  override;

            GameHub& m_game;
            
            gf::Action m_quitAction;
            
            gf::Font& m_cardsFont;
            gf::Font & m_rulesFont;

            gf::ExtendView m_principalView;
            gf::ViewContainer m_views;

            CardEntity m_cardEntity;
    };

}

#endif // TSL_ZOOM_CARD_SCENE_H