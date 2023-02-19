#include "ConnectionEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace tsl {

    ConnectionEntity::ConnectionEntity(gf::ResourceManager& resources)
    : m_paper(resources.getTexture("images/connection.png"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        m_paper.setSmooth(true);
    }
    
    void ConnectionEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        
        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
        paper.setAnchor(gf::Anchor::Center);

        gf::Text connection("Connexion", m_font);
        connection.setScale(scale * 1.6f);
        connection.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        connection.setPosition(coords.getRelativePoint({ 0.5f, 0.26f }));
        connection.setAnchor(gf::Anchor::Center);

        gf::Text hostname("Nom d hote", m_font);
        hostname.setScale(scale);
        hostname.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        hostname.setPosition(coords.getRelativePoint({ 0.5f, 0.44f }));
        hostname.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
        target.draw(connection, states);
        target.draw(hostname, states);
    
    }

}