#include "ButtonsGame.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>

namespace tsl {

    namespace {
        void setupButton(gf::TextButtonWidget& button) {
            button.setDefaultTextColor(gf::Color::fromRgba32(212,30,27,255));
            button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
            button.setDefaultBackgroundOutlineColor(gf::Color::Gray(0.7f)); 

            button.setSelectedTextColor(gf::Color::fromRgba32(212,30,27,255));
            button.setSelectedBackgroundColor(gf::Color::Gray(0.7f));
            button.setSelectedBackgroundOutlineColor(gf::Color::Gray(0.5f));
            
            button.setDisabledTextColor(gf::Color::Black);
            button.setDisabledBackgroundColor(gf::Color::Gray(0.7f));
            button.setDisabledBackgroundOutlineColor(gf::Color::Red); 

            button.setRadius(5.f);
            button.setBackgroundOutlineThickness(4.f);
            button.setAnchor(gf::Anchor::TopCenter);
        };
    }

    ButtonsGame::ButtonsGame(gf::ResourceManager& resources, ButtonChoice& choice, GameModel& model)
    : m_model(model)
    , m_endTurn("End turn", resources.getFont("fonts/Trajan-Color-Concept.otf"))
    , m_playCard("Activate card", resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        setupButton(m_endTurn);
        setupButton(m_playCard);

        m_endTurn.setCallback([&choice]() { choice = ButtonChoice::EndTurn; });
        m_container.addWidget(m_endTurn);

        m_playCard.setCallback([&choice]() { choice = ButtonChoice::PlayCard; });
        m_container.addWidget(m_playCard);
        
    }

    void ButtonsGame::pointTo(gf::Vector2f coords) {
        m_container.pointTo(coords);
    }

    void ButtonsGame::triggerAction() {
        m_container.triggerAction();
    }

    void ButtonsGame::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        m_endTurn.setString(m_model.getWord("End turn"));
        m_endTurn.setScale(scale*1.4f);
        m_endTurn.setPosition(coords.getRelativePoint({ 0.7f, 0.0305f}));
        m_endTurn.setAnchor(gf::Anchor::CenterRight);
        
        m_playCard.setString(m_model.getWord("Activate card"));
        m_playCard.setScale(scale*1.4f);
	    m_playCard.setPosition(coords.getRelativePoint({ 0.3f, 0.0305f}));
        m_playCard.setAnchor(gf::Anchor::CenterLeft);

        m_container.render(target, states);
        
    }

}