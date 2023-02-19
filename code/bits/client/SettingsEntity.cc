#include "SettingsEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace tsl {

    SettingsEntity::SettingsEntity(gf::ResourceManager& resources)
    : m_paper(resources.getTexture("images/settings.png"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        m_paper.setSmooth(true);
    }
    
    void SettingsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);

        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        
        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
        paper.setAnchor(gf::Anchor::Center);

        gf::Text settings("Parametres", m_font);
        settings.setScale(scale * 1.6f);
        settings.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        settings.setPosition(coords.getRelativePoint({ 0.5f, 0.26f }));
        settings.setAnchor(gf::Anchor::Center);

        gf::Text themes("Themes", m_font);
        themes.setScale(scale);
        themes.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        themes.setPosition(coords.getRelativePoint({ 0.5f, 0.38f }));
        themes.setAnchor(gf::Anchor::Center);

        gf::Text music("Musique", m_font);
        music.setScale(scale);
        music.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        music.setPosition(coords.getRelativePoint({ 0.5f, 0.49f }));
        music.setAnchor(gf::Anchor::Center);

        gf::Text sounds("Son", m_font);
        sounds.setScale(scale);
        sounds.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        sounds.setPosition(coords.getRelativePoint({ 0.5f, 0.6f }));
        sounds.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
        target.draw(settings, states);
        target.draw(themes, states);
        target.draw(music, states);
        target.draw(sounds, states);
    }

}