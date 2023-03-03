#include "MenuEntity.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Log.h>
namespace tsl {

    MenuEntity::MenuEntity(gf::ResourceManager& resources, MenuChoice& choice, GameModel& model)
    : m_model(model)
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    , m_playWidget(resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/ButtonSelected.png"))
    , m_rulesWidget(resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/ButtonSelected.png"))
    , m_quitWidget(resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/Button.png"), resources.getTexture("images/buttons/ButtonSelected.png"))
    , m_settingsWidget(resources.getTexture("images/Cog.png"), resources.getTexture("images/Cog.png"), resources.getTexture("images/Cog.png"))
    {
        m_playWidget.setCallback([&choice]() { choice = MenuChoice::Play; });
        m_container.addWidget(m_playWidget);

        m_rulesWidget.setCallback([&choice]() { choice = MenuChoice::Rules; });
        m_container.addWidget(m_rulesWidget);

        m_quitWidget.setCallback([&choice]() { choice = MenuChoice::Quit; });
        m_container.addWidget(m_quitWidget);

        m_settingsWidget.setCallback([&choice]() { choice = MenuChoice::Settings; });
        m_container.addWidget(m_settingsWidget);
    }

    void MenuEntity::pointTo(gf::Vector2f coords) {
        m_container.pointTo(coords);
    }

    void MenuEntity::triggerAction() {
        m_container.triggerAction();
    }
    
    void MenuEntity::update([[maybe_unused]] gf::Time time) {
        
        m_settingsWidget.setRotation(m_settingsWidget.getRotation()+0.01f);
    }

    void MenuEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        gf::Text title(m_model.getWord("Storm on chess"), m_font, 74);
        title.setScale(scale);
        title.setColor(gf::Color::White);
        title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
        title.setAnchor(gf::Anchor::Center);
        
        m_playWidget.setPosition(coords.getRelativePoint({0.5f, 0.4f}));
        m_playWidget.setScale(scale/2.5f);
        m_playWidget.setAnchor(gf::Anchor::Center);
        
        m_rulesWidget.setScale(scale/2.5f);
        m_rulesWidget.setPosition(coords.getRelativePoint({0.5f, 0.6f}));
        m_rulesWidget.setAnchor(gf::Anchor::Center);
    
        m_quitWidget.setScale(scale/2.5f);
        m_quitWidget.setPosition(coords.getRelativePoint({0.5f, 0.8f}));
        m_quitWidget.setAnchor(gf::Anchor::Center);

        const float min_scale = std::min(scale.x, scale.y)/4.f;

        m_settingsWidget.setScale(gf::Vector2f(min_scale));
        m_settingsWidget.setPosition(coords.getRelativePoint({0.9f, 0.9f }));
        m_settingsWidget.setAnchor(gf::Anchor::Center);
    
        target.draw(title, states);
        m_container.render(target, states);	
        renderTitleButton(target, states, { 0.5f, 0.4f }, m_model.getWord("Play"));
        renderTitleButton(target, states, { 0.5f, 0.6f }, m_model.getWord("Rules"));
        renderTitleButton(target, states, { 0.5f, 0.8f }, m_model.getWord("Quit"));
    }

    void MenuEntity::renderTitleButton(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f pos, std::string title) {
		gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

		gf::Text text(title, m_font, 35);
        text.setScale(scale);
        text.setColor(gf::Color::fromRgba32(252, 223, 0));;
        text.setPosition(coords.getRelativePoint(pos));
        text.setAnchor(gf::Anchor::Center);
    
        target.draw(text, states);
	}
}