#include "RulesScene.h"

#include "GameHub.h"

namespace tsl {

    RulesScene::RulesScene(GameHub& game)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_choice(RulesChoice::None)
    , m_rules(game.resources,  m_choice)
    {
        setClearColor(gf::Color::Black);
        
        addHudEntity(m_rules);
    }

    void RulesScene::doProcessEvent(gf::Event& event) {
        switch (event.type) {
            case gf::EventType::MouseMoved:
                m_rules.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
                break;

            case gf::EventType::MouseButtonPressed:
                m_rules.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                m_rules.triggerAction();
                break;

            default:
                break;
        }
    }

    void RulesScene::doUpdate([[maybe_unused]] gf::Time time) {
        if (!isActive()) {
			return;
		}

        switch (m_choice) {
            case RulesChoice::None:
                break;
            case RulesChoice::Before:
                break;
            case RulesChoice::After:
                break;
            case RulesChoice::Quit:
                m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
        }

        m_choice = RulesChoice::None;
    }

}