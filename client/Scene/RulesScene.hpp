#ifndef RULES_SCENE_H
#define RULES_SCENE_H

#include "../Entity/RulesEntity.hpp"

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Text.h>


struct GameHub;

class RulesScene : public gf::Scene {
public:
    RulesScene(GameHub& game);

    private:
        void doProcessEvent(gf::Event& event) override;
        void doHandleActions(gf::Window& window) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doShow() override;

    private:
        GameHub& m_game;

        gf::Action m_quitAction;
        gf::Action m_triggerAction;
        gf::TextButtonWidget m_quitButton;
        gf::WidgetContainer m_widgets;

        gf::Action m_upAction;
        gf::Action m_downAction;
        gf::Action m_fullscreenAction;
        
        RulesEntity m_rulesEntity;
};

#endif // RULES_SCENE_H