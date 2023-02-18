#include "MenuScene.h"

#include "GameHub.h"

namespace tsl {

    MenuScene::MenuScene(GameHub& game)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_choice(MenuChoice::None)
    , m_menu(game.resources,  m_choice)
    {
        setClearColor(gf::Color::Black);

        addHudEntity(m_menu);
    }

    void MenuScene::doProcessEvent(gf::Event& event) {
        switch (event.type) {
            case gf::EventType::MouseMoved:
                m_menu.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
                break;

            case gf::EventType::MouseButtonPressed:
                m_menu.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                m_menu.triggerAction();
                break;

            default:
                break;
        }
    }

    void MenuScene::doUpdate([[maybe_unused]] gf::Time time) {
        if (!isActive()) {
			return;
		}

        if (!m_game.loadingFinished()) {
            m_choice = MenuChoice::None;
            return;
        }

        switch (m_choice) {
            case MenuChoice::None:
                break;
            case MenuChoice::Play:
                m_game.replaceScene(*m_game.connection, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
            case MenuChoice::Rules:
                m_game.replaceScene(*m_game.rules, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
            case MenuChoice::Quit:
                m_game.popAllScenes();
                break;
            case MenuChoice::Settings:
                m_game.replaceScene(*m_game.settings, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
        }

        m_choice = MenuChoice::None;
    }

}