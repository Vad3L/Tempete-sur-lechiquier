#include "SettingsEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace tsl {

    SettingsEntity::SettingsEntity(gf::ResourceManager& resources, SettingsChoice& choice, GameModel& model)
    : m_model(model)
    , m_quitWidget(resources.getTexture("images/buttons/Close.png"), resources.getTexture("images/buttons/Close.png"), resources.getTexture("images/buttons/CloseSelected.png"))
    , m_paper(resources.getTexture("images/Settings.png"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        m_paper.setSmooth(true);

        m_quitWidget.setCallback([&choice]() { choice = SettingsChoice::Quit; });
        m_container.addWidget(m_quitWidget);
    }
    
    void SettingsEntity::pointTo(gf::Vector2f coords) {
        m_container.pointTo(coords);
    }

    void SettingsEntity::triggerAction() {
        m_container.triggerAction();
    }

    void SettingsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        
        m_quitWidget.setScale(scale);
        m_quitWidget.setPosition(coords.getRelativePoint({ 0.65f, 0.15f }));
        m_quitWidget.setAnchor(gf::Anchor::Center);

        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
        paper.setAnchor(gf::Anchor::Center);

        gf::Text settings(m_model.getWord("Settings"), m_font, 50);
        settings.setScale(scale);
        settings.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        settings.setPosition(coords.getRelativePoint({ 0.5f, 0.26f }));
        settings.setAnchor(gf::Anchor::Center);

        gf::Text themes(m_model.getWord("Theme"), m_font);
        themes.setScale(scale);
        themes.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        themes.setPosition(coords.getRelativePoint({ 0.5f, 0.38f }));
        themes.setAnchor(gf::Anchor::Center);

        gf::Text music(m_model.getWord("Music"), m_font);
        music.setScale(scale);
        music.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        music.setPosition(coords.getRelativePoint({ 0.5f, 0.49f }));
        music.setAnchor(gf::Anchor::Center);

        gf::Text sounds(m_model.getWord("Sound"), m_font);
        sounds.setScale(scale);
        sounds.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        sounds.setPosition(coords.getRelativePoint({ 0.5f, 0.6f }));
        sounds.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
        m_container.render(target, states);	
        target.draw(settings, states);
        target.draw(themes, states);
        target.draw(music, states);
        target.draw(sounds, states);
    }

}